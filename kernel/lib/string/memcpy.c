#include <stddef.h>

void *memcpy(void *dest, const void *src, size_t n) {
	const unsigned char *from  = src;
    unsigned char *to = dest;
    for (size_t i = 0; i < n; i++) {
        to[i] = from[i];
    }
    return dest;
}
