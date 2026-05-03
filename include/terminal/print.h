#ifndef PRINTK_H
#define PRINTK_H

// Kernel-space formatted print
void 	printk(const char *format, ...);

// Standard-style formatted print
int 	printf(const char *format, ...);

#endif