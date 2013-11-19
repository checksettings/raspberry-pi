#ifndef MAILBOX_H
#define MAILBOX_H

#include <stdint.h>


extern uint32_t mbox_read();
extern void mbox_write(uint32_t v);


#define MAILBOX_FULL      0x80000000
#define MAILBOX_EMPTY     0x40000000

#define MAILBOX_FB        1

#define MEMORY_OFFSET     0xC0000000


#endif /* MAILBOX_H */
