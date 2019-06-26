#include <stddef.h>

int memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char* buf1 = s1;
    const unsigned char* buf2 = s2;
    for (size_t i = 0; i < n; i++) {
        if (buf1[i] != buf2[i]) {
            return buf1[i] - buf2[i];
        }
    }
    return 0;
}