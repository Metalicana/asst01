#ifndef __KSTDIO_H
#define __KSTDIO_H
#include <stdint.h>
#include "../arch/stm32f446re/include/dev/usart.h"

//Extra method prototypes to help kscanf kprintf
int kstrcmp(uint8_t* str1, uint8_t* str2);
void kreverse(int st, int ed, uint8_t* s);
int power(int base, int p);
int32_t dig(char x);
char hexDig(int x);
int32_t stringToInt(uint8_t* s);
void intToString(uint8_t* s,int32_t n);
int32_t stringToHex(uint8_t *s);
void hexToString(uint8_t* s, int32_t n);
void floatToString(uint8_t *s, float n);
float stringToFloat(uint8_t *s);

void kprintf(uint8_t*,uint8_t*); 
void kscanf(uint8_t*,uint8_t *);

#endif /* KSTDIO */

