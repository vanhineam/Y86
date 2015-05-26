#include <stdio.h>
#include "bool.h"
#include "tools.h"

/* 
   Replace the return 0 with the code to perform the required function.
   Do not change the function prototypes.
   Be sure to check for errors in input. 
   Document EACH function with a function header (description, parameters,
   return).
*/

/**
 * getBits extracts the specified bits from a source integer.
 *
 * Parameters:
 *      low is the starting bit number
 *      high is the ending bit number
 *      source is the unsigned int to extract from
 *
 * Returns:
 *      The modified version of the source which contains the bits from low to
 *      high.
 */
unsigned int getBits(int low, int high, unsigned int source)
{
    if(low > high || low > 31 || low < 0 || high > 31 || high < 0)
    {
        return 0;
    }
    source = source << (31-high);
    source = source >> (31-high)+low;
    return source;
} 

/**
 * setBits sets the specified bits of an integer to 1
 *
 * Parameters:
 *      low is the starting bit number
 *      high is the ending bit number
 *      source is the unsigned int to modify
 *
 * Returns:
 *      The modified version of the source which now has 1 in the specified bits
 */
unsigned int setBits(int low, int high, unsigned int source)
{
    if(low > high || low > 31 || low < 0 || high > 31 || high < 0)
    {
        return source;
    }
    int mask = 0;
    int i = 0;
    for(i; i < high-low+1; ++i)
    {
        mask = mask << 1;
        mask += 1;
    }
    mask = mask << low;
    return source | mask;
}

/**
 * clearBits sets the specified bits of an integer to 0
 *
 * Parameters:
 *      low is the starting bit number
 *      high is the ending bit number
 *      source is the specified int to modify
 *
 * Returns:
 *      The modified version of the source which now has 0 in the specified bits
 *
 */
unsigned int clearBits(int low, int high, unsigned int source)
{
    if(low > high || low > 31 || low < 0 || high > 31 || high < 0)
    {
        return source;
    }
    int mask = 0;
    int i = 0;
    for(i; i < high-low+1; ++i)
    {
        mask = mask << 1;
        mask += 1;
    }
    mask = mask << low;
    mask = ~mask;
    return source & mask;
}

/**
 * assignOneBit sets a bit of an integer to a specified value(1 or 0)
 *
 * Parameters:
 *      bitNumber is the bit to be modified
 *      bitValue is the value of which to set the bitNumber equal to
 *      source is the specified int to modify
 *
 * Returns:
 *      The modified version of the source which now has the value set to the
 *      value of bitValue stored in the bitNumber location.
 */
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


/**
 * getByteNumber extracts a specfied byte(0-3) from a given number
 *
 * Parameters:
 *      byteNo is the byte being extracted(0-3)
 *      source is the number the byte will be extracted from
 *
 * Returns:
 *      The byte extracted from the source
 */
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


/**
 * putByteNumber sets a certain byte of a number to the specified value
 *
 * Parameters:
 *      byteNo is the byte to be set to in the #
 *      byteValue is the value to set the byteNo of source to
 *
 * Returns:
 *      A modified version of source with the byte value in the byte number location
 */
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

/**
 * buildWord takes 4 bytes and combines them into a single word
 *
 * Parameters:
 *      byte0 value to be stored in position 0-7
 *      byte1 value to be stored in position 8-15
 *      byte2 value to be stored in position 16-23
 *      byte3 value to be stored in position 24-31
 *
 * Returns:
 *      Built word from given bytes
 *
 */
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


/**
 * isNegative determines if a number is negative or positive
 *
 * Parameters:
 *      source is the number we are checking the sign on
 *
 * Returns:
 *      If it is negative return 1 otherwise return 0
 */
bool isNegative(unsigned int source)
{
    source = source >> 31;
    if(source)
    {
        return 1;
    }
    return 0;
}


/**
 * expandBits stores the binary representation of an int in a char array
 *
 * Parameters:
 *      source is the int to be stored
 *      bits[] is the array to store the int in
 *
 * Returns:
 *      None
 */
void expandBits(unsigned int source, char bits[36])
{
    int count = 0;
    int i = 34;
    for(i; i >= 0; --i)
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


/**
 * clearBuffer sets a location in memory and sets it to 0
 *
 * Parameters:
 *      *buff is the location to be cleared
 *      size is the amount to be cleared
 *
 * Returns:
 *      None
 */
void clearBuffer(char * buff, int size)
{
   int i = 0;
   for(i; i < size; ++i)
   {
       *buff = 0;
       *buff++;;
   }
}
