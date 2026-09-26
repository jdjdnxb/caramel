#include <stdint.h>
#include <stdarg.h>

#include <drivers/terminal/terminal.h>
#include <kprintf.h>

extern struct terminal kernel_terminal;

static const char lower_chars[] = "0123456789abcdef";
static const char upper_chars[] = "0123456789ABCDEF"; 

static void print_number(unsigned long long number, int radix, bool is_negative, bool uppercase, int min_digits) {
    char buffer[64];
    int pos = 0;
    const char* chars = uppercase ? upper_chars : lower_chars;

    do {
        unsigned long long rem = number % radix;
        number = number / radix;
        buffer[pos++] = chars[rem];
    } while (number > 0);

    while (pos < min_digits) {
        buffer[pos++] = '0';
    }

    if (is_negative) {
        buffer[pos++] = '-';
    }

    while (--pos >= 0) {
        terminal_put_char(&kernel_terminal, buffer[pos]);
    }
}

void kvprintf(const char* fmt, va_list args) {
    int state = PRINTF_STATE_START;
    int length = PRINTF_LENGTH_START;

    while (*fmt) {
        switch(state) {
        case PRINTF_STATE_START:
            if (*fmt == '%') {
                state = PRINTF_STATE_LENGTH;
            } else {
                terminal_put_char(&kernel_terminal, *fmt);
            }
            break;
        case PRINTF_STATE_LENGTH:
            if (*fmt == 'h') {
                length = PRINTF_LENGTH_SHORT;
                state = PRINTF_STATE_SHORT;
            } else if (*fmt == 'l') {
                length = PRINTF_LENGTH_LONG;
                state = PRINTF_STATE_LONG;
            } else {
                goto PRINTF_STATE_SPEC_;
            }
            break;
        case PRINTF_STATE_SHORT:
            if (*fmt == 'h') {
                length = PRINTF_LENGTH_SHORT_SHORT;
                state = PRINTF_STATE_SPEC;
            } else {
                goto PRINTF_STATE_SPEC_;
            }
            break;
        case PRINTF_STATE_LONG:
            if (*fmt == 'l') {
                length = PRINTF_LENGTH_LONG_LONG;
                state = PRINTF_STATE_SPEC;
            } 
            else {
                goto PRINTF_STATE_SPEC_;
            }
            break;

        case PRINTF_STATE_SPEC:
PRINTF_STATE_SPEC_:
            switch(*fmt) {
            case 'c':
                terminal_put_char(&kernel_terminal, (char)va_arg(args, int));
                break;
            case 's': {
                const char* s = va_arg(args, const char*);
                terminal_write(&kernel_terminal, s ? s : "(null)");
                break;
            }
            case '%':
                terminal_put_char(&kernel_terminal, '%');
                break;
            case 'd':
            case 'i': {
                bool is_neg = false;
                unsigned long long num = 0;

                if (length == PRINTF_LENGTH_LONG_LONG) {
                    long long int n = va_arg(args, long long int);
                    if (n < 0) { is_neg = true; num = (unsigned long long)0 - (unsigned long long)n; }
                    else num = n;
                } else if (length == PRINTF_LENGTH_LONG) {
                    long int n = va_arg(args, long int);
                    if (n < 0) { is_neg = true; num = (unsigned long)0 - (unsigned long)n; }
                    else num = n;
                } else {
                    int n = va_arg(args, int);
                    if (n < 0) { is_neg = true; num = (unsigned int)0 - (unsigned int)n; }
                    else num = n;
                }
                print_number(num, 10, is_neg, false, 0);
                break;
            }
            case 'u': {
                unsigned long long num;

                if (length == PRINTF_LENGTH_LONG_LONG)
                    num = va_arg(args, unsigned long long);
                else if (length == PRINTF_LENGTH_LONG)
                    num = va_arg(args, unsigned long);
                else
                    num = va_arg(args, unsigned int);

                print_number(num, 10, false, false, 0);
                break;
            }
            case 'x':
            case 'X': {
                uint64_t num;

                if (length == PRINTF_LENGTH_LONG_LONG) {
                    num = va_arg(args, unsigned long long);
                } else if (length == PRINTF_LENGTH_LONG) {
                    num = va_arg(args, unsigned long);
                } else {
                    num = va_arg(args, unsigned int);
                }

                terminal_write(&kernel_terminal, "0x");
                print_number(num, 16, false, *fmt == 'X', 0);

                break;
            }
            case 'o': {
                unsigned long long num = 0;
                if (length == PRINTF_LENGTH_LONG_LONG) {
                    num = va_arg(args, unsigned long long int);
                } else if (length == PRINTF_LENGTH_LONG) {
                    num = va_arg(args, unsigned long int);
                } else {
                    num = va_arg(args, unsigned int);
                }
                
                int radix = (*fmt == 'o') ? 8 : ((*fmt == 'u') ? 10 : 16);
                bool upper = (*fmt == 'X');
                print_number(num, radix, false, upper, 0);
                break;
            }
            case 'p': 
                uintptr_t ptr = (uintptr_t)va_arg(args, void *);
                terminal_write(&kernel_terminal, "0x");
                print_number(ptr, 16, false, false, sizeof(uintptr_t) * 2);
                break;
            default:
                terminal_put_char(&kernel_terminal, '%');
                terminal_put_char(&kernel_terminal, *fmt);
                break;
            }
            
            state = PRINTF_STATE_START;
            length = PRINTF_LENGTH_START;
            break;
        }
        fmt++;
    }
}

void kprintf(const char* fmt, ...) {
    va_list args;

    va_start(args, fmt);
    kvprintf(fmt, args);

    va_end(args);
}
