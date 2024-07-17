#ifndef EEP_H
#define EEP_H

#define SLAVE_READ		0xA1
#define SLAVE_WRITE		0xA0


void writeEEP(unsigned char address1,  unsigned char data);
unsigned char readEEP(unsigned char address1);

#endif
