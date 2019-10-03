#include <stdint.h>
#include <kernel/dev/i8042.h>
#include <kernel/kprintf.h>
#include "_i8042.h"
#include "../../util/port_io.h"

static uint8_t controller_type = I8042_TYPE_UNKNOWN;
static uint8_t devices_available = 0;

static uint8_t get_status() {
    uint8_t status = in_byte(I8042_STATUS_PORT);
    return status;
}

static void send_command(uint8_t command) {
    while (get_status() & I8042_STATUS_IN_BUFFER_FULL);
    out_byte(I8042_COMMAND_PORT, command);
}

static uint8_t read_data_byte() {
    // Wait until controller ready
    while (!(get_status() & I8042_STATUS_OUT_BUFFER_FULL));
    return in_byte(I8042_DATA_PORT);
}

static uint8_t read_data_byte_no_wait() {
    return in_byte(I8042_DATA_PORT);
}

static void write_data_byte(uint8_t data) {
    // Wait until controller ready
    while (get_status() & I8042_STATUS_IN_BUFFER_FULL);
    out_byte(I8042_DATA_PORT, data);
}

uint8_t ps2_get_availability_byte() {
    return devices_available;
}

void init_i8042() {
    kprintf("Init of i8042 controller...\n");
    // Disable devices
    send_command(I8042_COMMAND_DISABLE_PS_PORT_1);
    send_command(I8042_COMMAND_DISABLE_PS_PORT_2);
    kprintf("i8042: Devices disabled\n");
    // Clear buffer
    while (get_status() & I8042_STATUS_OUT_BUFFER_FULL) {
        read_data_byte_no_wait();
    }
    kprintf("i8042: Buffer cleared\n");
    // Disable interrupts and translation
    send_command(I8042_COMMAND_READ_CONF_BYTE);
    uint8_t conf_byte = read_data_byte();
    conf_byte &= ~I8042_CONF_INTERRUPT_PORT_1;
    conf_byte &= ~I8042_CONF_INTERRUPT_PORT_2;
    conf_byte &= ~I8042_CONF_TRANSLATION;
    send_command(I8042_COMMAND_WRITE_CONF_BYTE);
    write_data_byte(conf_byte);
    kprintf("i8042: Interrupts and translations disabled\n");
    // check does controller supports second channel?
    if (!(conf_byte & I8042_CONF_DISABLED_PORT_2)) {
        // We have disabled device 2, so it cannot be enabled if device supports second channel
        controller_type = I8042_TYPE_SINGLE_CHANNEL;
    }
    if (controller_type == I8042_TYPE_UNKNOWN) {
        send_command(I8042_COMMAND_ENABLE_PS_PORT_2);
        send_command(I8042_COMMAND_READ_CONF_BYTE);
        conf_byte = read_data_byte();
        if (!(conf_byte & I8042_CONF_DISABLED_PORT_2)) {
            controller_type = I8042_TYPE_DUAL_CHANNEL;
            send_command(I8042_COMMAND_DISABLE_PS_PORT_2);
        } else {
            controller_type = I8042_TYPE_SINGLE_CHANNEL;
        }
    }
    kprintf("i8042: Determined controller type - ");
    if (controller_type == I8042_TYPE_DUAL_CHANNEL) {
        kprintf("dual channel\n");
    } else {
        kprintf("single channel\n");
    }
    // Check what devices are available
    send_command(I8042_COMMAND_TEST_PS_PORT_1);
    if (read_data_byte() == 0) {
        devices_available |= I8042_DEV_1;
        kprintf("i8042: Device 1 available\n");
    }
    if (controller_type == I8042_TYPE_DUAL_CHANNEL) {
        send_command(I8042_COMMAND_TEST_PS_PORT_2);
        if (read_data_byte() == 0) {
            devices_available |= I8042_DEV_2;
            kprintf("i8042: Device 2 available\n");
        }
    }
}

void enable_device(uint8_t device_num) {
    send_command(I8042_COMMAND_READ_CONF_BYTE);
    uint8_t conf_byte = read_data_byte();
    if (device_num == I8042_DEV_1) {
        send_command(I8042_COMMAND_ENABLE_PS_PORT_1);
        conf_byte |= I8042_CONF_INTERRUPT_PORT_1;
    }
    if (device_num == I8042_DEV_2) {
        send_command(I8042_COMMAND_ENABLE_PS_PORT_2);
        conf_byte |= I8042_CONF_INTERRUPT_PORT_2;
    }
    send_command(I8042_COMMAND_WRITE_CONF_BYTE);
    write_data_byte(conf_byte);
    kprintf("i8042: Device %d was enabled\n", device_num);
}

int8_t ps2_device_write(uint8_t device_num, uint8_t byte) {
    if (device_num == I8042_DEV_2) {
        send_command(I8042_COMMAND_WRITE_TO_PS_PORT_2);
    }
    size_t counter_timeout = 0;
    while (counter_timeout < I8042_TIMEOUT_LIMIT && (get_status() & I8042_STATUS_IN_BUFFER_FULL)) {
        counter_timeout++;
    }
    if (counter_timeout == I8042_TIMEOUT_LIMIT) {
        return -1;
    }
    write_data_byte(byte);
    return 0;
}

size_t ps2_device_read(uint8_t *buffer, size_t count) {
    size_t count_read = 0;
    while (count_read < count && get_status() & I8042_STATUS_OUT_BUFFER_FULL) {
        buffer[count_read] = read_data_byte_no_wait();
    }
    return count;
}