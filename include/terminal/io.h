#ifndef IO_H
#define IO_H

// Write 1 byte to hardware port
void 						outb(unsigned short port, unsigned char val);

// Read 1 byte from hardware port
unsigned char 	inb(unsigned short port);
#endif