#include "stdio.h"
#include "framebuffer.h"


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
  consoleWriteChar(output_char);
  return (int32_t)output_char;
}

/**
 * Writes the given string followed by a newline to stdout.
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
    consoleWriteChar(*output_string++);
    ++string_length;
  }

  consoleWriteChar('\n');
  ++string_length;

  return string_length;
}
