#ifndef TOOLS_H
#define TOOLS_H

#include "bool.h"

/**
 * Extracts the specified bits from a source integer.
 * @param low the starting bit number. Range [0,31] and low <= high.
 * @param high the ending bit number. Range [0,31].
 * @param source the source integer.
 * @return An integer made of the bits [low,high], or 0 on an error.
 */
extern unsigned int getBits(int low, int high, unsigned int source);

/**
 * Sets the specified bits of an integer to 1.
 * @param low the starting bit number. Range [0,31] and low <= high.
 * @param high the ending bit number. Range [0,31].
 * @param source the source integer.
 * @return source, modified so that the bits [low,high] are set to 1.  On an
 * error, source is returned unmodified.
 */
extern unsigned int setBits(int low, int high, unsigned int source);

/**
 * Sets the specified bits of an integer to 0.
 * @param low the starting bit number. Range [0,31] and low <= high.
 * @param high the ending bit number. Range [0,31].
 * @param source the source integer.
 * @return source, modified so that the bits [low,high] are set to 0.  On an
 * error, source is returned unmodified.
 */
extern unsigned int clearBits(int low, int high, unsigned int source);

/**
 * Sets a single bit of an integer to a specifed value.
 * @param bitNumber the bit to be modified. Range [0,31].
 * @param bitValue the new value of the bit. Range [0,1].
 * @param source the source integer.
 * @return source, modified so that the bit is set as specified. On an error,
 * source is returned unmodified.
 */
extern unsigned int assignOneBit(int bitNumber, int bitValue,
    unsigned int source);

/**
 * Extracts a specified byte from an integer.
 * @param byteNo the byte to be extracted. Range [0,3].
 * @param source the source integer.
 * @return the byte extracted from source, or 0 on and error.
 */
extern unsigned char getByteNumber(int byteNo, unsigned int source);

/**
 * Set one byte in an integer to a specified value.
 * @param byteNo the byte to be set. Range [0,3].
 * @param byteValue the new value for the byte.
 * @param source the integer to be modified.
 * @return source, modified with the new byte value. On an error, source is
 * returned unmodified.
 */
extern unsigned int putByteNumber(int byteNo, unsigned char byteValue,
    unsigned int source);

/**
 * Takes 4 bytes and combines them to into a word.
 * @param byte0 the LSB of the word.
 * @param byte1 byte LSB + 1.
 * @param byte2 byte MSB - 1.
 * @param byte3 the MSB of the word.
 * @return the word built from the four bytes.
 */
extern unsigned int buildWord(unsigned char byte0, unsigned char byte1,
    unsigned char byte2, unsigned char byte3);

/**
 * Determines if an integer would be positive or negative using two's
 * completement.
 * @param source the integer to be tested.
 * @return true if source is negative.
 */
extern bool isNegative(unsigned int source);

/**
 * Stores the binary representation of an integer in a string.
 * @param source the int to be stored.
 * @param bits the array to store the string in.
 */
extern void expandBits(unsigned int source, char bits[36]);

/**
 * Zeroes a block of memory.
 * @param buff the location to be cleared.
 * @param size the number of bytes to be cleared.
 */
extern void clearBuffer(char * buff, int size);

#endif
