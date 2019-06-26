# Memory Management

This kernel currently supports only 32bit platforms. So it has 4gb of address space available. It uses paging for
managing virtual memory. Virtual memory layout is next:
0x0 - 0xC0000000 (first 3gb) - User space
0xC0000000 - _end_of_kernel symbol - Code, data, bss, stack of kernel itself.
_end_of_kernel - 0xFFFFFFFF - kernels heap, which grows up.

Physical allocation is done by using stack of free pages. Stack is placed and the bottom of kernel heap. During
initialization of MM, it is filled with free physical pages. Then it has functions to alloc and free page, and to map
it in some place in memory. Kernel has ksbrk and kbrk functions to control end of kernel heap.

## i386 details

On i386, in protected mode kernel must use some segmentation by gdt. We have flat segmentation, where all memory mapped
indeticaly by segments (currently we have two segments for kernel code and data, and two for user-space code and data).
Paging uses only 4kb pages. To be able to control current paging settings, last page directory (under 0xffbfffff
virtual) is mapped to page directory content itself, preserving order. It means that page page directory for first 4m
can be found at 0xffbfffff + 0, for second 4mb under 0xffbfffff + 4096, and so on. It should not be mixed up with
location of page tables in physical memory. Even if page table for 12mb-16mb is placed near to page table
for 3gb-3gb+4mb, they will be mapped to  0xffbfffff + 2 * 4096 and  0xffbfffff + 767 * 4096 respectively. This means
that last 4mb of virtual space is unusable - currently, it doesn't handled by platform independent mm.