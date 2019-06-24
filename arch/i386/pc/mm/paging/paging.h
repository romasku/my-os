#ifndef I386_PC_PAGING_H
#define I386_PC_PAGING_H

#include <stdint.h>

struct page_dir_entry {
    union {
        struct {
            int present: 1;
            int writable: 1;
            int user_accessable: 1;
            int write_through : 1;
            int cache_disalbed: 1;
            int accessed: 1;
            int ignored_1: 1;
            int huge_page: 1;
            int ignored_2: 4;
            unsigned int dir_table_addr: 20;
        };
        uint32_t data;
    };
};

struct page_table_entry {
    union {
        struct {
            int present: 1;
            int writable: 1;
            int user_accessable: 1;
            int write_through : 1;
            int cache_disalbed: 1;
            int accessed: 1;
            int dirty: 1;
            int pat_enabled: 1;
            int global: 1;
            int ignored: 3;
            unsigned int dir_table_addr: 20;
        };
        uint32_t data;
    };
};

void setup_paging();

#endif //I386_PC_PAGING_H
