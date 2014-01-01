#include "stdio.h"
#include "uart.h"
#include "framebuffer.h"  // can be removed if not needed -> all outputs always on uart


/* Defines the used output (UART/Framebuffer) */
static int32_t std_output = OUTPUT_UART;


void setStdOutput(int32_t std_out)
{
  std_output = std_out;
}

int32_t getStdOutput()
{
  return std_output;
}

/**
 * Writes the given character to stdout.
 * The character is casted to unsigned char.
 *
 * @param character The character for writing
 * @return The character written as unsigned char cast to int
 *
 */
int32_t putchar(int32_t character)
{
  uint8_t output_char = (uint8_t)character;
#ifdef FRAMEBUFFER_H
  if(!std_output)
    uartPutc(output_char);
  else
    consoleWriteChar(output_char);
#else
  uartPutc(output_char);
#endif
  return (int32_t)output_char;
}

/**
 * Writes the given string to stdout.
 *
 * @param output_string The string for writing
 * @return A non-negative number on success
 *
 */
int32_t puts(const char* output_string)
{
  int32_t string_length = 0;

  while(output_string && *output_string)
  {
#ifdef FRAMEBUFFER_H
    if(!std_output)
      uartPutc(*output_string++);
    else
      consoleWriteChar(*output_string++);
#else
    uartPutc(*output_string++);
#endif
    ++string_length;
  }

  return string_length;
}
