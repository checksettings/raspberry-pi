#include <stdint.h>

#ifndef MAILBOX_H
#define MAILBOX_H


extern uint32_t readMailbox(uint32_t channel);
extern void writeMailbox(uint32_t channel, uint32_t v);


#endif /* MAILBOX_H */
