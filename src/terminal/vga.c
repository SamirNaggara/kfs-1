#include "../../include/terminal/vga.h"
#include "../../include/terminal/io.h"

/**
 * Combines foreground and background colors into a single 8-bit attribute byte.
 * The background color occupies the high 4 bits, and the foreground occupies the low 4 bits.
 * Result: [ BBBB FFFF ]
 */
unsigned char vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

/**
 * Combines an ASCII character and a color attribute into a 16-bit VGA cell.
 * Due to x86 little-endianness, the character goes to the low byte (0-7) 
 * and the color goes to the high byte (8-15).
 * Result: [ AAAAAAAA CCCCCCCC ]
 */
unsigned short vga_entry(unsigned char uc, unsigned char color) {
    return (unsigned short)uc | (unsigned short)color << 8;
}

/**
 * Updates the hardware cursor position on the screen.
 * It talks to the VGA Controller using I/O ports:
 * 0x3D4: Address Register (used to select which internal register to write to)
 * 0x3D5: Data Register (used to send the actual value)
 */
void vga_update_cursor(int x, int y) {
    // Calculate the linear 16-bit position (index = y * width + x)
    unsigned short pos = y * VGA_WIDTH + x;
    
    // Send the lower 8 bits of the position to register 0x0F
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(pos & 0xFF));
    
    // Send the upper 8 bits of the position to register 0x0E
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((pos >> 8) & 0xFF));
}