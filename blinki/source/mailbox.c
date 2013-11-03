#include "mailbox.h"

uint32_t mbox_read()
{
  uint32_t r = 0;
  do {
    while (ReadMmReg32(MAIL_BASE, MAIL_STATUS) & MAIL_EMPTY); //wait for data
    r = ReadMmReg32(MAIL_BASE, MAIL_READ); //Read the data
  } while ((r & 0xF) != MAIL_FB); //Loop until we received something from the
  //frame buffer channel
  return r & 0xFFFFFFF0;
}

void mbox_write(uint32_t v)
{
  v += MEMORY_OFFSET;
  while (ReadMmReg32(MAIL_BASE, MAIL_STATUS) & MAIL_FULL); //wait for space
  //Write the value to the frame buffer channel
  WriteMmReg32(MAIL_BASE, MAIL_WRITE, MAIL_FB | (v & 0xFFFFFFF0));
}
