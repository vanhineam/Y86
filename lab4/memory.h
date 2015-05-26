#define MEMSIZE 1024     //1024 words of memory
#ifndef MEMORY_H
#define MEMORY_H

/**
 * Gets the byte in memory at the specified address
 *
 * Parameters:
 *  address is the location in memory to read the byte
 *  memError is set to TRUE if the address is not valid and FALSE if valid
 *
 * Return:
 *  If the address is invalid returns 0, otherwise return the value of the byte
 *  at the location "address"
 */
unsigned char getByte(int address, bool * memError);

/**
 * Stores a value passed in the specified byte in memory
 *
 * Parameter:
 *  address is the location in memory to store the value
 *  value is the value to be stored in memory
 *  memError is set to TRUE if the address is valid and FALSE if not valid
 *
 *  Return:
 *    None
 */
void putByte(int address, unsigned char value, bool * memError);

/**
 * Zeros the memory
 *
 */
void clearMemory();

/**
 * Gets the word that is stored in memory at the specified address
 *
 * Parameters:
 *  address is the location in memory to read from
 *  memError is set to TRUE if the address is not valid and FALSE if valid
 *
 * Return:
 *  If the address is invalid returns 0, otherwise return the value of the word
 *  at the location "address"
 */
unsigned int getWord(int address, bool * memError);

/**
 * Stores a word passed in the specified memory location
 *
 * Parameter:
 *  address is the location in the memory to store the word
 *  value is the word to be stored in memory
 *  memError is set TRUE if not valid and FALSE otherwise
 *
 * Return:
 *  None
 */
void putWord(int address, unsigned int value, bool * memError);
#endif 

