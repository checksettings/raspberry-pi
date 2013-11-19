#ifndef MAILBOX_H
#define MAILBOX_H

#include <stdint.h>


extern uint32_t readmailbox(uint32_t channel);
extern void writemailbox(uint32_t channel, uint32_t v);


#define MAILBOX_FULL      0x80000000
#define MAILBOX_EMPTY     0x40000000

#define MEMORY_OFFSET     0xC0000000


#endif /* MAILBOX_H */
