#include "../include/kstdio.h"
//#include <stdio.h>
#include <stdint.h>
/**
* first argument define the type of string to kprintf and kscanf, 
* %c for charater
* %s for string, 
* %d for integer
* %x hexadecimal
* %f for floating point number
*/

// for comparing formats for printf/scanf
int kstrcmp(uint8_t* str1, uint8_t* str2)
{
    uint8_t* tempstr1 = str1;
    while((*str1) && (*str2))
    {
        if((*str1)!=(*str2))
        {
            str1 = tempstr1;
            return 0; 
        }
        str1++;
        str2++;
    }
    if(!(*str1) && !(*str2))
    {
        str1 = tempstr1;
        return 1; 
    }
    str1 = tempstr1;
    return 0;
}

// for reversing a string 
void kreverse(int st, int ed, uint8_t* s)
{
    if(st>=ed) return;

    char tempst = s[st];
    s[st] = s[ed];
    s[ed] = tempst;

    kreverse(st+1,ed-1,s);
}

// for calculating powers of base in int/hex
int power(int base, int p)
{
    int ret = 1;
    
    while(p--)
    {
        ret*=base;
    }

    return ret;
}

//functions for parsing hex digits to integers and vice versa

int32_t dig(char x)
{
    if(x>='A' && x<='F')return 10 + (int32_t)x-(int32_t)'A';
    else return (int32_t)x-(int32_t)'0';
}

char hexDig(int x)
{
    if(x>=10)return (char)('A'+ x%10);
    return (char)(x+'0');
}

// for converting a string of characters to decimal integer

int32_t stringToInt(uint8_t* s)
{
    //int len = 0;
    int p = 1;
    int32_t n = 0;
    int len = 0;
    for(len = 0; s[len]; len++);
    for(int i=len-1;i>=0;i--)
    {
        if(s[i]=='-')continue;
        n += (p*dig(s[i]));
        p*=10;
    }
    if(s[0]=='-')return -n;
    else return n;
}

// for converting a decimal integer to a string

void intToString(uint8_t* s,int32_t n)
{
    int pos = 0;
    int neg = 0;
    if(n==0)s[pos++]='0';
    else if(n<0)
    {
        n=-n;
        neg = 1;
    }
    while(n>0)
    {
        int dig = n % 10;
        s[pos++] = dig + '0';
        n/=10;
    }
    if(neg == 1)
    {
        s[pos++] = '-';
    }
    s[pos] = 0;
    pos--;

    kreverse(0,pos,s);
}

// for converting a string of characters to a hexadecimal integer

int32_t stringToHex(uint8_t *s)
{
    //assuming the string starts with 0x
    int p = 1;
    int32_t n = 0;
    int len = 0;
    for(len = 0; s[len]; len++);
    for(int i=len-1;i>=0;i--)
    {
        if(s[i]=='x')break;
        n += (p*dig(s[i]));
        p*=16;
    }
    if(s[0]=='-')n=-n;
    return n;

}

// for converting hex integer to string

void hexToString(uint8_t* s, int32_t n)
{
    int pos = 0;
    int neg = 0;
    if(n==0)
    {
        s[pos++]='0';
    }
    else if(n<0)
    {
        n=-n;
        neg = 1;
    }
    while(n>0)
    {
        int dig = n % 16;
        s[pos++] = hexDig(dig);
        n/=16;
    }
    s[pos++]='x';
    s[pos++]='0';
    if(neg == 1)
    {
        s[pos++] = '-';
    }
    s[pos] = 0;
    pos--;

    kreverse(0,pos,s);
}

// for converting a floating point number to a string

void floatToString(uint8_t *s, float n)
{
    int neg = 0;
    int pos = 0;
    if(n<0.0)neg=1;
    int x = (int)n;
    int y = (int)((n-(float)x)*100000);
    uint8_t newS1[20],newS2[20];
    for(int q=0;q<5;q++)
    {
        newS1[q] = '0' + y%10;
        y/=10;
    }
    newS1[5]=0;
    kreverse(0,4,newS1);
    newS2[pos++]='.';
    if(x == 0)newS2[pos++] = '0';
    while(x>0)
    {
        newS2[pos++] = '0'+ x%10;
        x/=10;
    }
    if(neg==1)newS2[pos++] = '-';
    newS2[pos]=0;
    pos--;
    kreverse(0,pos,newS2);
    int iter = 0;
    for(int q=0;newS2[q];q++)s[iter++]=newS2[q];
    for(int q=0;newS1[q];q++)s[iter++]=newS1[q];
    s[iter++]=0;
}

// for converting a string of characters to a floating point number 

float stringToFloat(uint8_t *s)
{
    int i = 0;
    for(i=0;s[i]!='.';i++);
    int pw = power(10,i-1);
    float x = 0.0;
    for(int q=0;q<i;q++)
    {
        x += (float)(dig(s[q])*pw);
        pw/=10;
    }
    float pwf = 0.1;
    for(int q=i+1,p=0;p<5;q++,p++)
    {
        x += (float)(dig(s[q])*pwf);
        pwf/=10.0;
    }
    return x;
}

void kprintf(uint8_t *format, uint8_t* outvar)
{
    if(kstrcmp(format, (uint8_t*) "%c"))
    {
        UART_SendChar(USART2, (uint8_t)(*outvar));
    }
    else if(kstrcmp(format, (uint8_t*) "%s"))
    {
        _USART_WRITE(USART2, outvar);
    }
    else if(kstrcmp(format, (uint8_t*) "%d"))
    {
        uint8_t s[20];
        int32_t n = *((int32_t *)outvar);
        intToString(s,n);
        _USART_WRITE(USART2, (uint8_t*) s);       
    }
    else if(kstrcmp(format, (uint8_t*) "%x"))
    {
        uint8_t s[20];
        int32_t n = *((int32_t *)outvar);
        hexToString(s,n);
        _USART_WRITE(USART2, (uint8_t*) s);       
    }
    else if(kstrcmp(format, (uint8_t*) "%f"))
    {
        uint8_t s[20];
        float n = *((float *)outvar);
        floatToString(s,n);
        _USART_WRITE(USART2, (uint8_t*) s);
    }
}

void kscanf(uint8_t *format, uint8_t* invar)
{
    if(kstrcmp(format, (uint8_t*) "%c"))
    {
        _USART_READ(USART2, invar, 2);
    }
    else if(kstrcmp(format, (uint8_t*) "%s"))
    {
        _USART_READ(USART2,invar,1000);
    }
    else if(kstrcmp(format, (uint8_t*) "%d"))
    {
        uint8_t temp[20];
        _USART_READ(USART2,temp,1000);
         int32_t num = stringToInt(temp);
         *((int32_t*)(invar)) = num;
    }
    else if(kstrcmp(format, (uint8_t*) "%x"))
    {
        uint8_t temp[20];
        _USART_READ(USART2,temp,1000);
        int32_t num = stringToHex(temp);
        *((int32_t*)(invar)) = num;
    }
    else if(kstrcmp(format, (uint8_t*) "%f"))
    {
        uint8_t temp[20];
        _USART_READ(USART2,temp,1000);
        float  num = stringToFloat(temp);
        *((float*)(invar)) = num;  
    }
}

