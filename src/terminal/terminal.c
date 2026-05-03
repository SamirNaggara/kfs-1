#include "../../include/terminal/vga.h"
#include "../../include/terminal/terminal.h"

/**
 * Internal state of the terminal driver.
 * x, y: Current cursor coordinates.
 * color: Current VGA color attribute (8-bit).
 * buffer: Pointer to the VGA text buffer (Memory-mapped I/O at 0xB8000).
 */
struct terminal {
    int x;
    int y;
    unsigned char color;
    volatile unsigned short *buffer;
};

static struct terminal term;

/**
 * Initializes the terminal state, maps the video memory,
 * and clears the screen with the default color.
 */
void terminal_initialize(void) {
    term.x = 0;
    term.y = 0;
    term.color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    term.buffer = (unsigned short*) 0xB8000;

    // The screen size is 80x25 = 2000 cells.
    const int total_size = VGA_WIDTH * VGA_HEIGHT;

    for (int i = 0; i < total_size; i++) {
        term.buffer[i] = vga_entry(' ', term.color);
    }

    vga_update_cursor(term.x, term.y);
}

/**
 * Updates the current text color attribute for subsequent write operations.
 */
void terminal_set_color(enum vga_color fg, enum vga_color bg) {
    term.color = vga_entry_color(fg, bg);
}

/**
 * Shifts all rows up by one when the cursor reaches the bottom of the screen.
 * Clears the last line and resets the cursor position.
 */
static void terminal_scroll(void) {
    // Move every row (except the first) one line up
    for (int y = 1; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            int to = (y - 1) * VGA_WIDTH + x;
            int from = y * VGA_WIDTH + x;
            term.buffer[to] = term.buffer[from];
        }
    }

    // Clear the last line with empty spaces
    for (int x = 0; x < VGA_WIDTH; x++) {
        int index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
        term.buffer[index] = vga_entry(' ', term.color);
    }

    term.y = VGA_HEIGHT - 1;
    vga_update_cursor(term.x, term.y);
}

/**
 * Places a single character in the VGA buffer at the current cursor position.
 * Handles newline '\n' characters and triggers scrolling if necessary.
 */
void terminal_put_char(char c) {
    if (c == '\n') {
        term.x = 0;
        term.y++;
    } else {
        int index = term.y * VGA_WIDTH + term.x;
        term.buffer[index] = vga_entry((unsigned char)c, term.color);
        term.x++;
    }

    // Line wrap logic
    if (term.x >= VGA_WIDTH) {
        term.x = 0;
        term.y++;
    }

    // Screen scroll logic
    if (term.y >= VGA_HEIGHT) {
        terminal_scroll();
    }
    
    vga_update_cursor(term.x, term.y);
}

/**
 * Writes a null-terminated string to the terminal.
 */
void terminal_write(const char* data) {
    for (int i = 0; data[i] != '\0'; i++) {
        terminal_put_char(data[i]);
    }
}