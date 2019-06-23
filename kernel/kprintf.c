#include <kernel/kprintf.h>
#include <kernel/tty.h>
#include <stdarg.h>
#include <kernel/panic.h>

// TODO: create more efficient version without syncing screen after each character printed
// TODO: support more printf features

void print_uint(unsigned int number) {
    if (number == 0) {
        tty_putchar('0');
        return;
    }
    char buf[10];
    size_t i = 0;
    while (number > 0) {
        buf[i++] = (number % 10) + '0';
        number /= 10;
    }
    for (size_t j = 0; j < i; j++) {
        tty_putchar(buf[i - j - 1]);
    }
}

void kprintf(const char *format, ...) {
    va_list argp;
    va_start(argp, format);
    while (*format != '\0') {
        if (*format == '%') {
            format++;
            if (*format == '%') {
                tty_putchar('%');
                format++;
            } else if (*format == 'd' || *format == 'i') {
                int number = va_arg(argp, int);
                if (number == 0) {
                    kprintf("zero\n");
                }
                if (number < 0) {
                    tty_putchar('-');
                    number = -number;
                }
                print_uint((unsigned int) number);
                format++;
            } else if (*format == 'u') {
                unsigned int number = va_arg(argp, unsigned int);
                print_uint(number);
                format++;
            } else {
                panic("kprintf do not support such control symbol.");
            }
        } else {
            tty_putchar(*format);
            format++;
        }

    }
}