#include "../../include/terminal/print.h"
#include "../../include/terminal/terminal.h"

/* --- INTERNAL UTILITIES FOR NUMBER-TO-STRING CONVERSION --- */

/**
 * Converts an unsigned integer to a string representation in a given base.
 * @param num: The number to convert.
 * @param base: The numerical base (10 for decimal, 16 for hexadecimal).
 * * Logic: It extracts digits using the modulo operator (%) and stores them 
 * in a temporary buffer. Since they are extracted in reverse order, 
 * it prints them from the last filled index back to 0.
 */
static void print_number(unsigned int num, int base) {
    if (num == 0) {
        terminal_put_char('0');
        return;
    }

    char buffer[32]; // Sufficient for 32-bit integers in any base
    int i = 0;
    char *digits = "0123456789abcdef";

    // Extract digits by remainder (modulo)
    while (num > 0) {
        buffer[i++] = digits[num % base];
        num /= base;
    }

    // Print digits in correct order (reverse of extraction)
    while (i > 0) {
        i--;
        terminal_put_char(buffer[i]);
    }
}

/**
 * Handles signed integers by checking for a negative sign.
 * If negative, prints '-' and converts the absolute value.
 */
static void print_signed_number(int num) {
    if (num < 0) {
        terminal_put_char('-');
        // Convert to unsigned to handle the largest negative integer correctly
        print_number((unsigned int)(-num), 10);
    } else {
        print_number((unsigned int)num, 10);
    }
}

/**
 * The core engine of the print system.
 * Parses the format string and fetches arguments from the stack.
 * * Supported format specifiers:
 * %c - Character
 * %s - String
 * %d - Signed Decimal
 * %x - Unsigned Hexadecimal
 * %% - Literal percent sign
 */
static void core_print(__builtin_va_list args, const char *format) {
    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%' && format[i + 1] != '\0') {
            i++;
            switch (format[i]) {
                case 'c': 
                    // Note: chars are promoted to int when passed through '...'
                    terminal_put_char((char)__builtin_va_arg(args, int)); 
                    break;
                case 's': {
                    const char *s = __builtin_va_arg(args, const char *);
                    terminal_write(s ? s : "(null)");
                    break;
                }
                case 'd': 
                    print_signed_number(__builtin_va_arg(args, int)); 
                    break;
                case 'x': 
                    print_number(__builtin_va_arg(args, unsigned int), 16); 
                    break;
                case '%': 
                    terminal_put_char('%'); 
                    break;
                default:
                    // Print the unknown specifier as literal text
                    terminal_put_char('%');
                    terminal_put_char(format[i]);
                    break;
            }
        } else {
            terminal_put_char(format[i]);
        }
    }
}

/* --- PUBLIC API FUNCTIONS --- */

/**
 * Kernel-space print function.
 * Uses variadic arguments to format and display messages.
 */
void printk(const char *format, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, format);
    
    // Future expansion: we could also write to a serial port or kernel log buffer
    core_print(args, format);
    
    __builtin_va_end(args);
}

/**
 * Standard-compliant printf wrapper.
 * Currently returns 0, but could be updated to return the number of chars written.
 */
int printf(const char *format, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, format);
    
    core_print(args, format);
    
    __builtin_va_end(args);
    
    return 0; 
}