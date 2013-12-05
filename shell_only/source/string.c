#include "string.h"


int32_t strcmp (const char* l, const char* r)
{
  for(; *l == *r; ++l, ++r)
    if(*l == 0)
      return 0;
  return *(uint8_t*)l < *(uint8_t*)r ? -1 : 1;
}

uint32_t strlen (const char* str)
{
  const char *pos = str;

  while ( *pos )
    ++pos;

  return ( pos - str );
}
