#include <kernel/kprintf.h>
#include "gdt.h"

uint64_t pack_limit(uint32_t limit) {
    uint64_t limit_64 = limit;
    return (limit_64 & 0xffff) | ((limit_64 & 0xf0000) << 32);
}

uint64_t pack_base(uint32_t base) {
    uint64_t base_64 = base;
    return ((base_64 & 0xffff) << 16) | ((base_64 & 0xff0000) << 16) | ((base_64 & 0xff000000) << 32);
}


uint64_t pack_flags(struct gdt_entry_data data) {
    uint64_t result = 0;
    // Access byte part
    result |= (data.present & 1ll) << 47;
    result |= (data.privilege_level & 3ll) << 45;
    result |= ((!data.type_system) & 1ll) << 44;
    result |= ((data.executable) & 1ll) << 43;
    if (data.executable) {
        result |= (data.direction_grows_down & 1ll) << 42;
        result |= (data.code_readable & 1ll) << 41;
    } else {
        result |= (data.conforming & 1ll) << 42;
        result |= (data.data_writable & 1ll) << 41;
    }
    result |= (data.accessed & 1ll) << 40;
    // Flags part
    result |= (data.limit_is_in_pages & 1ll) << 55;
    result |= (data.size_32_bit & 1ll) << 54;
    return result;
}

void write_gdt_entry(struct gdt_entry_data data, gdt_entry *entry) {
    *entry = pack_limit(data.limit) | pack_base(data.base) | pack_flags(data);
}

