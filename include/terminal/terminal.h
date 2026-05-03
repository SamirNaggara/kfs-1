#ifndef TERMINAL_H
#define TERMINAL_H

#include "vga.h"

// Initialize state and clear screen
void terminal_initialize(void);

// Set color for next characters
void terminal_set_color(enum vga_color fg, enum vga_color bg);

// Print one character (handles \n and scrolling)
void terminal_put_char(char c);

// Print a null-terminated string
void terminal_write(const char* data);

#endif