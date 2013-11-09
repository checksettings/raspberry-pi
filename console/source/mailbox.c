#include "mailbox.h"


/* Mailbox memory addresses */
static volatile uint32_t* MAILBOX_READ   = (uint32_t*) 0x2000B880;
static volatile uint32_t* MAILBOX_STATUS = (uint32_t*) 0x2000B898;
static volatile uint32_t* MAILBOX_WRITE  = (uint32_t*) 0x2000B8A0;


uint32_t readmailbox(uint32_t channel)
{
  uint32_t r = 0;
  do {
    while (*MAILBOX_STATUS & MAILBOX_EMPTY); //wait for data
    r = *MAILBOX_READ; //Read the data
  } while ((r & 0xF) != channel); //Loop until we received something from the
  //frame buffer channel
  return r & 0xFFFFFFF0;
}

void writemailbox(uint32_t channel, uint32_t v)
{
  v += MEMORY_OFFSET;
  while (*MAILBOX_STATUS & MAILBOX_FULL); //wait for space
  //Write the value to the frame buffer channel
  *MAILBOX_WRITE = channel | (v & 0xFFFFFFF0);
}
