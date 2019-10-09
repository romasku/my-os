#ifndef MYOS_TSS_H
#define MYOS_TSS_H

struct tss_segment {
    uint16_t previous_task_link;
    uint16_t pad1;
    uint32_t esp0; // stack base and stack_segment for ring 0
    uint16_t ss0;
    uint16_t pad2;
    uint32_t esp1; // stack base and stack_segment for ring 1
    uint16_t ss1;
    uint16_t pad3;
    uint32_t esp2; // stack base and stack_segment for ring 2
    uint16_t ss2;
    uint16_t pad4;
    uint32_t cr3;
    uint32_t pad5[18];
    uint16_t pad6;
    uint16_t io_map_address;
} __attribute__((packed));
struct tss_segment tss_segment;

void init_tss();
void activate_tss();
extern void _enable_tss(uint16_t segment_id);

#endif //MYOS_TSS_H
