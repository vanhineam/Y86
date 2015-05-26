#include "tools.h"

#include <stdio.h>

unsigned int getBits(int low, int high, unsigned int source)
{
    if(low > high || low > 31 || low < 0 || high > 31 || high < 0)
    {
        return 0;
    }

    source = source << (31 - high);
    source = source >> ((31 - high) + low);
    return source;
} 

unsigned int setBits(int low, int high, unsigned int source)
{
    if(low > high || low > 31 || low < 0 || high > 31 || high < 0)
    {
        return source;
    }
    
    int mask = 0;
    for(int i = 0; i < (high - low + 1); ++i)
    {
        mask = mask << 1;
        mask += 1;
    }

    mask = mask << low;
    return source | mask;
}

unsigned int clearBits(int low, int high, unsigned int source)
{
    if(low > high || low > 31 || low < 0 || high > 31 || high < 0)
    {
        return source;
    }

    int mask = 0;
    for(int i = 0; i < high-low+1; ++i)
    {
        mask = mask << 1;
        mask += 1;
    }

    mask = mask << low;
    mask = ~mask;
    return source & mask;
}

unsigned int assignOneBit(int bitNumber, int bitValue, unsigned int source)
{
    if(bitValue > 1 || bitValue < 0)
    {
        return source;
    }

    int mask = 1;
    mask = mask << bitNumber;
    if(bitValue)
    {
        return setBits(bitNumber,bitNumber,source);
    }
    else
    {
        return clearBits(bitNumber,bitNumber,source);
    }
}

unsigned char getByteNumber(int byteNo, unsigned int source)
{
    switch(byteNo)
    {
        case(0):
        {
            return getBits(0,7,source);
            break;
        }
        case(1):
        {
            return getBits(8,15,source);
        }
        case(2):
        {
            return getBits(16,23,source);
        }
        case(3):
        {
            return getBits(24,31,source);
        }
    }
    return 0;
}

unsigned int putByteNumber(int byteNo, unsigned char byteValue, 
                           unsigned int source)
{
    if(byteNo < 0 || byteNo > 3)
    {
        return source;
    }

    switch(byteNo)
    {
        case(0):
        {
            source = clearBits(0,7,source);
            break;
        }
        case(1):
        {
            source = clearBits(8,15,source);
            break;
        }
        case(2):
        {
            source = clearBits(16,23,source);
            break;
        }
        case(3):
        {
            source = clearBits(24,31,source);
            break;
        }
    }
    int mask = byteValue << (byteNo*8);
    return source | mask;

}

unsigned int buildWord(unsigned char byte0, unsigned char byte1, 
                       unsigned char byte2, unsigned char byte3)
{
    int result = 0;
    result = result | byte3 << 24;
    result = result | byte2 << 16;
    result = result | byte1 << 8;
    result = result | byte0;
    return result;
}

bool isNegative(unsigned int source)
{
    source = source >> 31;
    if(source)
    {
        return 1;
    }
    return 0;
}

void expandBits(unsigned int source, char bits[36])
{
    int count = 0;
    for(int i = 34; i >= 0; --i)
    {
        if(i == 8 || i == 17 || i == 26)
        {
            bits[i] = ' ';
        }
        else
        {
            int result = getBits(count,count,source);
            bits[i] = '0' + result;
            count += 1;
        }
    }
    bits[35] = 0;
}

void clearBuffer(char * buff, int size)
{
   for(int i = 0; i < size; ++i)
   {
     buff[i] = 0;
   }
}
