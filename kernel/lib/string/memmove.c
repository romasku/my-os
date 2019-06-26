#include <stddef.h>

void *memmove(void *dest, const void *src, size_t n) {
    const unsigned char *from  = src;
    unsigned char *to = dest;
    if (from < to) {
        // We can safely copy it
        for (size_t i = 0; i < n; i++) {
            to[i] = from[i];
        }
    } else {
        // We can safely copy it from end
        for(size_t i = n; i != 0; i--) {
            to[i - 1] = from[i - 1];
        }
    }

	return dest;
}
