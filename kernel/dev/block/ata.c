#include <kernel/dev/block/ata.h>
#include <stddef.h>
#include <stdint.h>
#include <kernel/arch/ide.h>
#include <kernel/lib/malloc.h>
#include <kernel/kprintf.h>
#include <kernel/panic.h>
#include "_ata.h"

static uint8_t get_status(uint8_t bus) {
    return ide_read_io_register_byte(bus, ATA_IO_REGISTER_STATUS);
}

static void perform_select_drive(uint8_t bus, uint8_t is_master) {
    uint8_t drive_register = ide_read_io_register_byte(bus, ATA_IO_REGISTER_DRIVE);
    if (is_master) {
        drive_register &= ~ATA_USE_SLAVE_BIT;
    } else {
        drive_register |= ATA_USE_SLAVE_BIT;
    }
    ide_write_io_register_byte(bus, ATA_IO_REGISTER_DRIVE, drive_register);
}

static void enable_lba(uint8_t bus) {
    uint8_t drive_register = ide_read_io_register_byte(bus, ATA_IO_REGISTER_DRIVE);
    drive_register |= ATA_USE_LBA_BIT;
    ide_write_io_register_byte(bus, ATA_IO_REGISTER_DRIVE, drive_register);
}

static void do_delay() {
    // Do 4 reads to delay a little (yes, we have to do this)
    ide_read_control_register_byte(0, ATA_CONTROL_REGISTER_ALT_STATUS);
    ide_read_control_register_byte(0, ATA_CONTROL_REGISTER_ALT_STATUS);
    ide_read_control_register_byte(0, ATA_CONTROL_REGISTER_ALT_STATUS);
    ide_read_control_register_byte(0, ATA_CONTROL_REGISTER_ALT_STATUS);
}

void init_drive_if_available(uint8_t bus, uint8_t is_master) {
    // We need to read this before we send any data to this bus
    uint8_t status = get_status(bus);
    if (status == ATA_STATUS_BUS_FLOATING) {
        return; // We nothing connected to this bus, skip
    }
    perform_select_drive(bus, is_master);
    do_delay();
    // Prepare for IDENTIFY
    ide_write_io_register_byte(bus, ATA_IO_REGISTER_SECTOR_COUNT, 0);
    ide_write_io_register_byte(bus, ATA_IO_REGISTER_SECTOR_NUMBER_0, 0);
    ide_write_io_register_byte(bus, ATA_IO_REGISTER_SECTOR_NUMBER_1, 0);
    ide_write_io_register_byte(bus, ATA_IO_REGISTER_SECTOR_NUMBER_2, 0);
    // Send IDENTIFY
    ide_write_io_register_byte(bus, ATA_IO_REGISTER_COMMAND, ATA_COMMAND_IDENTIFY);
    // Check that drive exists
    status = get_status(bus);
    if (status == 0) {
        return;
    }
    while (status & ATA_STATUS_BSY && (!(status & ATA_STATUS_DRQ) || !(status && ATA_STATUS_ERR))) {
        status = get_status(bus);
    }
    if (status & ATA_STATUS_ERR) {
        return; // Cannot IDENTIFY
    }
    uint16_t *buf = malloc(ATA_IDENTIFY_SIZE * sizeof(uint16_t));
    for (uint16_t i = 0; i < ATA_IDENTIFY_SIZE; i++) {
        buf[i] = ide_read_io_register_word(bus, ATA_IO_REGISTER_DATA);
    }
    uint64_t blocks_count = *(uint64_t *)(buf + 100);
    kprintf("ATA: Detected drive with size %dmb\n", blocks_count * ATA_SECTOR_SIZE / 1024 / 1024);
    // Store data about drive
    ata_drives[ata_drives_cnt].bus = bus;
    ata_drives[ata_drives_cnt].master = is_master;
    ata_drives[ata_drives_cnt].sector_count = blocks_count;
    ata_drives_cnt++;
    // Enable LBA for future use
    enable_lba(bus);
    free(buf);
}

void init_ata() {
    for (uint8_t bus = 0; bus < 2; bus++) {
        init_drive_if_available(bus, 0);
        init_drive_if_available(bus, 1);
    }
}

static void set_lba_and_sector_count(uint8_t bus, uint64_t lba, uint16_t sector_count) {
    ide_write_io_register_byte(bus, ATA_IO_REGISTER_SECTOR_COUNT, sector_count >> 8); // Sec Cnt high
    ide_write_io_register_byte(bus, ATA_IO_REGISTER_SECTOR_NUMBER_0, lba >> 24);      // LBA4
    ide_write_io_register_byte(bus, ATA_IO_REGISTER_SECTOR_NUMBER_1, lba >> 32);      // LBA5
    ide_write_io_register_byte(bus, ATA_IO_REGISTER_SECTOR_NUMBER_2, lba >> 40);      // LBA6
    ide_write_io_register_byte(bus, ATA_IO_REGISTER_SECTOR_COUNT, sector_count);          // Sec Cnt low
    ide_write_io_register_byte(bus, ATA_IO_REGISTER_SECTOR_NUMBER_0, lba);                // LBA1
    ide_write_io_register_byte(bus, ATA_IO_REGISTER_SECTOR_NUMBER_1, lba >> 8);       // LBA2
    ide_write_io_register_byte(bus, ATA_IO_REGISTER_SECTOR_NUMBER_2, lba >> 16);      // LBA3
}

int32_t ata_read(struct ata_drive *drive, uint64_t start_sector, uint16_t sector_count, uint8_t *buffer) {
    if (start_sector + sector_count > drive->sector_count) {
        panic("ATA: trying to read non existing sector\n");
    }
    perform_select_drive(drive->bus, drive->master);
    do_delay();
    // Send sector count and LBA
    set_lba_and_sector_count(drive->bus, start_sector, sector_count);
    // Send READ command
    ide_write_io_register_byte(drive->bus, ATA_IO_REGISTER_COMMAND, ATA_COMMAND_READ_SECTORS_EXT);
    uint16_t *buffer_ptr = (uint16_t *) buffer;
    while (sector_count > 0) {
        uint8_t status = get_status(drive->bus);
        while (status & ATA_STATUS_BSY && (!(status & ATA_STATUS_DRQ) || !(status & ATA_STATUS_ERR))) {
            status = get_status(drive->bus);
        }
        if (status & ATA_STATUS_ERR) {
            uint8_t error = ide_read_io_register_byte(drive->bus, ATA_IO_REGISTER_ERROR);
            kprintf("ATA: Error %d!\n", error);
            return -1; // Read failed
        }
        for (uint16_t i = 0; i < ATA_SECTOR_SIZE / sizeof(uint16_t); i++) {
            *buffer_ptr = ide_read_io_register_word(drive->bus, ATA_IO_REGISTER_DATA);
            buffer_ptr++;
        }
        sector_count--;
        if (sector_count > 0) {
            do_delay();
        }
    }
    return 0;
}

int32_t ata_write(struct ata_drive *drive, uint64_t start_sector, uint16_t sector_count, uint8_t *data) {
    if (start_sector + sector_count > drive->sector_count) {
        panic("ATA: trying to read non existing sector\n");
    }
    perform_select_drive(drive->bus, drive->master);
    do_delay();
    // Send sector count and LBA
    set_lba_and_sector_count(drive->bus, start_sector, sector_count);
    // Send READ command
    ide_write_io_register_byte(drive->bus, ATA_IO_REGISTER_COMMAND, ATA_COMMAND_WRITE_SECTORS_EXT);
    uint16_t *data_ptr = (uint16_t *) data;
    while (sector_count > 0) {
        uint8_t status = get_status(drive->bus);
        while (status & ATA_STATUS_BSY && (!(status & ATA_STATUS_DRQ) || !(status & ATA_STATUS_ERR))) {
            status = get_status(drive->bus);
        }
        if (status & ATA_STATUS_ERR) {
            uint8_t error = ide_read_io_register_byte(drive->bus, ATA_IO_REGISTER_ERROR);
            kprintf("ATA: Error %d!\n", error);
            return -1; // Read failed
        }
        for (uint16_t i = 0; i < ATA_SECTOR_SIZE / sizeof(uint16_t); i++) {
            ide_write_io_register_word(drive->bus, ATA_IO_REGISTER_DATA, *data_ptr);
            data_ptr++;
        }
        sector_count--;
        if (sector_count > 0) {
            do_delay();
        }
    }
    return 0;
}