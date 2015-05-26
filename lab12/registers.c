#include "registers.h"

#include "bool.h"
#include "tools.h"
#include <string.h>

// stores the processor registers
static unsigned int registers[REGSIZE];
// stores the processor condition codes
static unsigned int CC;

unsigned int getRegister(int regNum)
{
  if (regNum < 0 || regNum >= REGSIZE)
  {
    return 0;
  }

  return registers[regNum];
}

void setRegister(int regNum, unsigned int regValue)
{
  if (regNum < 0 || regNum >= REGSIZE)
  {
    return;
  }

  registers[regNum] = regValue;
}

void clearRegisters()
{
  clearBuffer((char*)&registers, sizeof(int) * REGSIZE);
}

void setCC(unsigned int bitNumber, unsigned int value)
{
  if (bitNumber != ZF && bitNumber != SF && bitNumber != OF)
  {
    return;
  }

  CC = assignOneBit(bitNumber, value, CC);
}

unsigned int getCC(unsigned int bitNumber)
{
  if (bitNumber != ZF && bitNumber != SF && bitNumber != OF)
  {
    return 0;
  }

  return getBits(bitNumber, bitNumber, CC);
}
