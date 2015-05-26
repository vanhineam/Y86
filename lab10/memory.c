#include "memory.h"

#include "tools.h"

#include <stdio.h>

// storage for the memory
static unsigned int mem[MEMSIZE];

/**
 * Retrieves the specified word from memory.
 * @param address the word address to read.
 * @param memError set to true if an error occurs accessing memory.
 * @return the word read from memory, or 0 on a memory error.
 */
static unsigned int fetch(int address, bool* memError);

/**
 * Stores a word in memory.
 * @param address the word address to write to.
 * @param memError set to true if an error occurs accessing memory.
 */
static void store(int address, unsigned int value, bool* memError);

unsigned int fetch(int address, bool * memError)
{
  if (address < 0 || address >= MEMSIZE)
  {
    *memError = TRUE;
    return 0;
  }

  *memError = FALSE;
  return mem[address];
}

void store(int address, unsigned int value, bool * memError)
{
  if (address < 0 || address >= MEMSIZE)
  {
    *memError = TRUE;
    return;
  }

  *memError = FALSE;
  mem[address] = value;
}

unsigned char getByte(int address, bool * memError)
{
  if (address < 0 || address >= (MEMSIZE * 4))
  {
    *memError = TRUE;
    return 0;
  }

  int byteNo = address % 4;
  unsigned int value = fetch(address / 4, memError);

  return getByteNumber(byteNo, value);
}

void putByte(int address, unsigned char value, bool * memError)
{
  if (address < 0 || address >= (MEMSIZE * 4))
  {
    *memError = TRUE;
    return;
  }

  int byteNo = address % 4;
  unsigned int word  = fetch (address / 4, memError);

  word = putByteNumber(byteNo, value, word);
  store(address / 4, word, memError);
}

void clearMemory()
{
  clearBuffer((char *) &mem, sizeof(int) * MEMSIZE);
}

unsigned int getWord(int address, bool * memError)
{
  if (address % 4 != 0)
  {
    *memError = TRUE;
    return 0;
  }

  return fetch(address / 4, memError);
}

void putWord(int address, unsigned int value, bool * memError)
{
  if (address % 4 != 0)
  {
    *memError = TRUE;
    return;
  }

  store(address / 4, value, memError);
}
