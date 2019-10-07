#ifndef MYOS_IDE_H
#define MYOS_IDE_H

#include <stdint.h>
#include <stddef.h>

#define ATA_SECTOR_SIZE 512

struct ata_drive {
    uint8_t bus;
    uint8_t master;
    uint64_t sector_count;
};

struct ata_drive ata_drives[4];
static uint8_t ata_drives_cnt = 0;

void init_ata();
int32_t ata_read(struct ata_drive *drive, uint64_t start_sector, uint16_t sector_count, uint8_t *buffer);
int32_t ata_write(struct ata_drive *drive, uint64_t start_sector, uint16_t sector_count, uint8_t *data);

#endif //MYOS_IDE_H
