#ifndef __MY_MACRO_H
#define __MY_MACRO_H

#include <stdint.h>

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

#define ENABLE 1
#define DISABLE 0

//clear and set bit
#define sbi(port,bit) \
	(port) |= (1 << (bit))
#define cbi(port,bit) \
	(port) &= ~(1 << (bit))
//check bit
#define bit_is_set(sfr,bit) \
	((uint8_t)sfr & (uint8_t)(1<<bit)) 

#endif /* __MY_MACRO_H */