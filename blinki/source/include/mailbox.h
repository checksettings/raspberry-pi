#ifndef MAILBOX_H
#define MAILBOX_H

#include <stdint.h>


extern uint32_t mbox_read();
extern void mbox_write(uint32_t v);


#define MAIL_BASE      0x2000B880 /* This is the base address for the mailbox registers */
#define MAIL_READ      0x00 /* We read from this register */
#define MAIL_WRITE     0x20 /* This is where we write to; it is actually the read/write of the other mailbox */
#define MAIL_STATUS    0x18 /* Status register for this mailbox */

#define MAIL_FULL      0x80000000
#define MAIL_EMPTY     0x40000000

#define MAIL_FB        1 /* The frame buffer uses channel 1 */

#define MEMORY_OFFSET  0xC0000000
#define MAILBOX_ADDRESS (uint32_t*)0x9000

#define ReadMmReg32(base,offset)        (*((volatile uint32_t*)((uint32_t)base + (uint32_t)offset)))
#define WriteMmReg32(base,offset,dest)  (*((volatile uint32_t*)((uint32_t)base + (uint32_t)offset))) = (dest)

#endif /* MAILBOX_H */
