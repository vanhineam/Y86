#ifndef MEMORY_H
#define MEMORY_H

#include "bool.h"

#define MEMSIZE 1024     //1024 words of memory

/**
 * Gets the byte in memory at the specified address.
 * @param address the byte address to read.
 * @param memError set to true if an error occurs accessing memory.
 * @return the value of the byte read, or 0 on a memory error.
 */
extern unsigned char getByte(int address, bool * memError);

/**
 * Stores a value passed in the specified byte in memory.
 * @param address the byte address to write.
 * @param value the value to be stored in memory.
 * @param memError set to true if an error occurs accessing memory.
 */
extern void putByte(int address, unsigned char value, bool * memError);

/**
 * Zeros the entire memory.
 */
extern void clearMemory();

/**
 * Gets the word that is stored in memory at the specified address.
 * @param address the byte address to read.  Must be word-aligned.
 * @param memError set to true if an error occurs accessing memory.
 * @return the value of the word read, or 0 on a memory error.
 */
extern unsigned int getWord(int address, bool * memError);

/**
 * Stores a word in the specified memory location.
 * @param address the byte address to write.  Must be word-aligned.
 * @param value the word to be stored in memory.
 * @param memError set to true if an error occurs accessing memory.
 */
extern void putWord(int address, unsigned int value, bool * memError);

#endif 

