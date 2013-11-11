#include "stdio.h"
#include "stdarg.h"
#include "framebuffer.h"


typedef struct c_string
{
  // start pointer
  char *start;

  // working pointer
  char *ptr;

  // length of the string
  uint32_t length;

  // actual allocated size for the string
  uint32_t size;

} c_string;

unsigned char const ZEROPAD = 1;  /* pad with zero */
unsigned char const SIGN    = 2;  /* unsigned/signed long */
unsigned char const PLUS    = 4;  /* show plus */
unsigned char const SPACE   = 8;  /* space if plus */
unsigned char const LEFT    = 16; /* left justified */
unsigned char const SPECIAL = 32; /* 0x */
unsigned char const LARGE   = 64; /* use 'ABCDEF' instead of 'abcdef' */

//----------------------------------------------------------------------
uint32_t strlen ( const char *str )
{
  const char *pos = str;

  while ( *pos )
  {
    ++pos;
  }

  return ( pos - str );
}

//----------------------------------------------------------------------
int32_t atoi(const char *string)
{
  int32_t number = 0;
  int32_t base = 10;
  int32_t negative = 0;

  if(*string == '-')
  {
    negative = 1;
    ++string;
  }
  else
    if(*string == '+')
      ++string;

  while(*string >= '0' && *string <= '9')
  {
    number *= base;
    number += *string - '0';
    ++string;
  }

  if(negative)
    number *= -1;

  return number;
}





/**
 * Writes a number of fill chars into a string
 */
void writeFillChars(c_string *output_string, int size, char fill )
{
  while( size-- > 0 )
  {
    *output_string->ptr++ = fill;
    ++output_string->length;
  }
}

/**
 * Writes a number into a string using the given parameters.
 *
 * @param output_string Structure containing information for the output string
 * @param number The number for writing
 * @param base The base of the number
 * @param size The size (in digits) for output
 * @param precision Precision for output
 * @param type Output type
 *
 */
void writeNumber(c_string *output_string, uint32_t number,
                 uint32_t base, unsigned int size,
                 unsigned int precision, unsigned char type)
{
	char c;
  char sign,tmp[70];
	const char *digits;
	static const char small_digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	static const char large_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	unsigned int i;

	digits = (type & LARGE) ? large_digits : small_digits;
	if (type & LEFT)
  {
		type &= ~ZEROPAD;
    size = 0;  //no padding then
  }
	if (base < 2 || base > 36)
		return;
	c = (type & ZEROPAD) ? '0' : ' ';
	sign = 0;
	if (type & SIGN) {
		if (((int) number) < 0) {
			sign = '-';
			number = - (int) number;
		} else if (type & PLUS) {
			sign = '+';
		} else if (type & SPACE) {
			sign = ' ';
		}
	}
	i = 0;
	if (number == 0)
		tmp[i++]='0';
	else while (number != 0)
  {
		tmp[i++] = digits[number%base];
    number /= base;
  }
	if (sign) {
    tmp[i++] = sign;
  }
	if (type & SPECIAL) {
    precision = 0;
		if (base==8) {
        tmp[i++] = '0';
		} else if (base==16) {
        tmp[i++] = digits[33];
        tmp[i++] = '0';
      }
	}

  if (precision > size)
    precision = size;

  while (size-- - precision > i)
  {
    *output_string->ptr++ = c;
    ++output_string->length;
  }

  while (precision-- > i)
  {
    *output_string->ptr++ = '0';
    ++output_string->length;
  }

	while (i-- > 0)
  {
    *output_string->ptr++ = tmp[i];
    ++output_string->length;
  }
}

//----------------------------------------------------------------------
/**
 * Writes output to stdout.
 * A detailed description of the format is given in the
 * 'Linux Programmer's Manual'.
 *
 * @param format A string containing the output format, followed by an\
 argument list containing different variables for output
 * @return The number of characters printed or the number of characters that\
 would have been printed if the output was truncated, a negative\
 value is returned on failure
 *
 */
int printf(const char *format, ...)
{
  c_string output_string;
  int character_count = 256;


  char buffer[character_count];

  output_string.start = (char *) &buffer;
  output_string.ptr = output_string.start;
  output_string.length = 0;
  output_string.size = character_count;

  va_list args;

  va_start(args, format);

  // format handling taken from vkprintf
  while (format && *format)
  {
    if(!character_count)
    {
      break;
    }

    if (*format == '%')
    {
      int width = 0;
      unsigned char flag = 0;
      ++format;
      switch (*format)
      {
        case '-':
          flag |= LEFT;
          ++format;
          break;
        case '+':
          flag |= PLUS;
          ++format;
          break;
        case '0':
          flag |= ZEROPAD;
          ++format;
          break;
        default:
          break;
      }
      uint32_t c = 0;
      char num[4];
      for(; *format > '0' && *format <= '9'; ++format, ++c)
        if( c < 4 )
          num[c] = *format;
      num[c < 4 ? c : 3] = 0;
      if( c )
      {
        width = atoi(num);


        if(character_count < width)
          width = character_count;
      }

      switch (*format)
      {
        case '%':
          *output_string.ptr++ = *format;

          --character_count;
          ++output_string.length;
          break;

        case 's':
        {
          char const *string_arg = va_arg(args, char const*);
          int len = strlen(string_arg);

          // we should align right -> fill with spaces
          if( !((flag & LEFT) && (len < width)) )
          {
            character_count -= width - len;
            if( character_count < 0 )
            {
              break;
            }

            writeFillChars(&output_string, width - len, ' ');
          }

          // now print the string
          while(string_arg && *string_arg)
          {
            if(!character_count)
            {
              break;
            }


            *output_string.ptr++ = *string_arg++;
            --character_count;
            ++output_string.length;
          }

          // and some fillchars if aligned on the left
          if( flag & LEFT && len < width )
          {
            character_count -= width - len;
            if( character_count < 0 )
            {
              break;
            }

            writeFillChars(&output_string, width - len, ' ');
          }

          break;
        }

        //signed decimal
        case 'd':
          writeNumber(&output_string,(unsigned int) va_arg(args,int),10,width, 0, flag | SIGN);
          break;

        //octal
        case 'o':
          writeNumber(&output_string,(unsigned int) va_arg(args,unsigned int),8,width, 0, flag | SPECIAL);
          break;

        //unsigned
        case 'u':
          writeNumber(&output_string,(unsigned int) va_arg(args,unsigned int),10,width, 0, flag );
          break;

        case 'x':
          writeNumber(&output_string,(unsigned int) va_arg(args,unsigned int),16,width, 0, flag | SPECIAL);
          break;

        case 'X':
          writeNumber(&output_string,(unsigned int) va_arg(args,unsigned int), 16, width, 0, flag | SPECIAL | LARGE);
          break;

        case 'c':
          *output_string.ptr++ = (char) va_arg(args,unsigned int);
          ++output_string.length;
          break;

        default:
          va_arg(args,void);
          break;
      }

    }
    else
    {
      *output_string.ptr++ = *format;
      --character_count;
      ++output_string.length;
    }

    ++format;
  }

  va_end(args);


//-------TODO---------
// function for framebuffer
//  character_count = write(STDOUT_FILENO,(void*) output_string.start, output_string.length);

  const char* string_ptr = output_string.start;
  uint32_t to_write = output_string.length;
  do
  {
    consoleWriteChar(*string_ptr++);
  } while(--to_write);


  return (int) character_count;
}




