#include <stdint.h>


uint64_t __aeabi_uidivmod(uint32_t value, uint32_t divisor)
{
  uint64_t answer =0;
  uint32_t i;
  for(i=0;i<32; i++)
  {
    if((divisor << (31 -i))>>(31-i)==divisor)
    {
      if(value>=divisor << (31-i))
      {
        value -= divisor << (31-i);
        answer |= 1 << (31-i);
        if(value ==0)
          break;
      }
    }
  }

  answer |= (uint64_t)value << 32;
  return answer;
}

int32_t __aeabi_idiv(int32_t numerator, int32_t denominator) 
{
  int32_t neg_result = (numerator ^ denominator) & 0x80000000;
  int32_t result = __aeabi_uidivmod ((numerator < 0) ? -numerator : numerator, (denominator < 0) ? -denominator : denominator);
  return neg_result ? -result : result;
}

uint32_t __aeabi_uidiv(uint32_t numerator, uint32_t denominator) 
{
  return __aeabi_uidivmod (numerator, denominator);
  
}

