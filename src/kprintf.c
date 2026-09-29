#include <stdint.h>
#include <stdarg.h>

#include <drivers/terminal/terminal.h>
#include <kprintf.h>

extern struct terminal kernel_terminal;

static const char lower_chars[] = "0123456789abcdef";
static const char upper_chars[] = "0123456789ABCDEF"; 

static void print_number(unsigned long long number, int radix, bool is_negative, bool uppercase, int min_digits) 
{
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

void kvprintf(const char *fmt, va_list args)
{
    while (*fmt)
    {
        if (*fmt != '%')
        {
            terminal_put_char(&kernel_terminal, *fmt++);
            continue;
        }

        fmt++;

        if (*fmt == '%')
        {
            terminal_put_char(&kernel_terminal, '%');
            fmt++;
            continue;
        }

        int length = PRINTF_LENGTH_START;
        int width = 0;
        bool zero_pad = false;

        if (*fmt == '0')
        {
            zero_pad = true;
            fmt++;
        }

        while (*fmt >= '0' && *fmt <= '9')
        {
            width = width * 10 + (*fmt - '0');
            fmt++;
        }

        if (*fmt == 'h')
        {
            length = PRINTF_LENGTH_SHORT;
            fmt++;

            if (*fmt == 'h')
            {
                length = PRINTF_LENGTH_SHORT_SHORT;
                fmt++;
            }
        }
        else if (*fmt == 'l')
        {
            length = PRINTF_LENGTH_LONG;
            fmt++;

            if (*fmt == 'l')
            {
                length = PRINTF_LENGTH_LONG_LONG;
                fmt++;
            }
        }
        
        if (*fmt == '\0')
            break;

        switch (*fmt)
        {
        case 'c':
        {
            int value = va_arg(args, int);
            terminal_put_char(&kernel_terminal, (char)value);
            break;
        }

        case 's':
        {
            const char *string = va_arg(args, const char *);
            terminal_write(
                &kernel_terminal,
                string ? string : "(null)"
            );
            break;
        }

        case 'd':
        case 'i':
        {
            bool is_negative = false;
            unsigned long long number;

            if (length == PRINTF_LENGTH_LONG_LONG)
            {
                long long value = va_arg(args, long long);

                if (value < 0)
                {
                    is_negative = true;
                    number = 0ULL - (unsigned long long)value;
                }
                else
                {
                    number = (unsigned long long)value;
                }
            }
            else if (length == PRINTF_LENGTH_LONG)
            {
                long value = va_arg(args, long);

                if (value < 0)
                {
                    is_negative = true;
                    number = 0ULL - (unsigned long long)value;
                }
                else
                {
                    number = (unsigned long long)value;
                }
            }
            else
            {
                int value = va_arg(args, int);

                if (value < 0)
                {
                    is_negative = true;
                    number = 0ULL - (unsigned int)value;
                }
                else
                {
                    number = (unsigned int)value;
                }
            }

            print_number(
                number,
                10,
                is_negative,
                false,
                zero_pad ? width : 0
            );

            break;
        }

        case 'u':
        {
            unsigned long long number;

            if (length == PRINTF_LENGTH_LONG_LONG)
            {
                number = va_arg(args, unsigned long long);
            }
            else if (length == PRINTF_LENGTH_LONG)
            {
                number = va_arg(args, unsigned long);
            }
            else
            {
                number = va_arg(args, unsigned int);
            }

            print_number(
                number,
                10,
                false,
                false,
                zero_pad ? width : 0
            );

            break;
        }

        case 'x':
        case 'X':
        {
            unsigned long long number;

            if (length == PRINTF_LENGTH_LONG_LONG)
            {
                number = va_arg(args, unsigned long long);
            }
            else if (length == PRINTF_LENGTH_LONG)
            {
                number = va_arg(args, unsigned long);
            }
            else
            {
                number = va_arg(args, unsigned int);
            }

            print_number(
                number,
                16,
                false,
                *fmt == 'X',
                zero_pad ? width : 0
            );

            break;
        }

        case 'o':
        {
            unsigned long long number;

            if (length == PRINTF_LENGTH_LONG_LONG)
            {
                number = va_arg(args, unsigned long long);
            }
            else if (length == PRINTF_LENGTH_LONG)
            {
                number = va_arg(args, unsigned long);
            }
            else
            {
                number = va_arg(args, unsigned int);
            }

            print_number(
                number,
                8,
                false,
                false,
                zero_pad ? width : 0
            );

            break;
        }

        case 'p':
        {
            uintptr_t pointer = (uintptr_t)va_arg(args, void *);

            terminal_write(&kernel_terminal, "0x");

            print_number(
                pointer,
                16,
                false,
                false,
                sizeof(uintptr_t) * 2
            );

            break;
        }

        default:
            terminal_put_char(&kernel_terminal, '%');
            terminal_put_char(&kernel_terminal, *fmt);
            break;
        }

        fmt++;
    }
}

void kprintf(const char* fmt, ...) 
{
    va_list args;

    va_start(args, fmt);
    kvprintf(fmt, args);

    va_end(args);
}
