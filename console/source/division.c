#include <stdint.h>
uint32_t __aeabi_uidivmod(uint32_t a, uint32_t b)
{
   uint32_t c,d,e;
   c = a/b;
   d = c*b;
   e = a - d;
   return e;
}

int __aeabi_idiv(int numerator, int denominator) 
{
  int neg_result = (numerator ^ denominator) & 0x80000000;
  int result = __aeabi_uidivmod ((numerator < 0) ? -numerator : numerator, (denominator < 0) ? -denominator : denominator);
  return neg_result ? -result : result;
}
unsigned __aeabi_uidiv(unsigned numerator, unsigned denominator) 
{
  return __aeabi_uidivmod (numerator, denominator);
}


