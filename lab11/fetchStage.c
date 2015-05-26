#include "fetchStage.h"

#include "bool.h"
#include "tools.h"
#include "registers.h"
#include "memory.h"
#include "instructions.h"
#include "decodeStage.h"
#include "memoryStage.h"
#include "writebackStage.h"

#include <stdlib.h>
#include <assert.h>

#define LOW_BIT_START 0
#define LOW_BIT_END 3
#define HIGH_BIT_START 4
#define HIGH_BIT_END 7

//F register holds the input for the fetch stage. 
static fregister F;

/**
 * Determines the status code for fetched instruction
 * @param memError if there has been a memory error.
 * @param validInstruction if the instruction was valid.
 * @param icode the icode.
 * @return SAOK if normal execution, SHLT if the instruction was a IHALT,
 * SINS if invalid instruction and SADR if invalid address
 */
static unsigned int getStatus(bool memError, bool validInstruction,
    unsigned int icode);

/**
 * Returns the current value of the PC from the F register.
 * @return the current value of the PC.
 */
static unsigned int fetchPC(pipelineForward pf);

/**
 * Reads and returns the icode from the provided address.
 * @param byte the byte containing the icode
 * @return The icode.
 */
static unsigned int getIcode(unsigned char byte);

/**
 * Reads and returns the ifun from the provided address.
 * @param byte the byte containing the ifun.
 * @return The ifun.
 */
static unsigned int getIfun(unsigned char byte);

/**
 * Tests the provided instruction to determine its validity.
 * @param icode the icode to be tested.
 * @return true if the icode is valid.
 */
static bool validInstruction(unsigned int icode);

/**
 * Gets the address of the next instruction.
 * @param pc the current value of the pc.
 * @param icode the current icode.
 * @return the address of the next instruction.
 */
static unsigned int getValP(unsigned int pc, unsigned int icode);

/**
 * Predicts the next value of the pc.
 * @param pc the current value of the pc.
 * @param icode the icode of the current instruction.
 * @param valC the valC of the current instruction.
 * @return the predicted value of the pc.
 */
static unsigned int predictPC(unsigned int pc, unsigned int icode,
    unsigned int valC);

/**
 * Determines if the current instruction requires a register byte.
 * @param icode the icode to be tested.
 * @return true if the register byte is needed.
 */
static bool needRegByte(unsigned int icode);

/**
 * Retrieves the rA value, based on the icode.
 * @param icode the current icode.
 * @param byte the register byte of the instruction.
 * @return the rA value.
 */
static unsigned int getRA(unsigned int icode, unsigned char byte);

/**
 * Retrieves the rB value, based on the icode.
 * @param icode the current icode.
 * @param byte the register byte of the instruction.
 * @return the rB value.
 */
static unsigned int getRB(unsigned int icode, unsigned char byte);

/**
 * Determines if the current instruction requires a valC.
 * @param icode the current icode.
 * @return true if the valC is needed.
 */
static bool needValC(unsigned int icode);

/**
 * Retrieves the valC from memory.
 * @param pc the current value of the pc.
 * @param icode the current icode.
 * @param memError records any memory errors.
 * @return the valC read, or 0 on a memory error.
 */
static unsigned int getValC(unsigned int pc, unsigned int icode,
    bool* memError);

unsigned int fetchPC(pipelineForward pf)
{
  // branch misprediction after jump
  if (pf.M_icode == IJXX && !pf.M_Cnd)
  {
    return pf.M_valA;
  }
  // completion of RET
  else if (pf.W_icode == IRET)
  {
    return pf.W_valM;
  }
  // use default predicted pc
  else
  {
    return F.predPC;
  }
}

unsigned int getIcode(unsigned char byte)
{
  return getBits(HIGH_BIT_START, HIGH_BIT_END, byte);
}

unsigned int getIfun(unsigned char byte)
{
  return getBits(LOW_BIT_START, LOW_BIT_END, byte);
}

bool validInstruction(unsigned int icode)
{
  switch (icode)
  {
    case IHALT:
    case INOP:
    case IRRMOVL:
    case IIRMOVL:
    case IMRMOVL:
    case IOPL:
    case IJXX:
    //case ICMOVXX: // duplicate of IRRMOVL
    case ICALL:
    case IRET:
    case IPUSHL:
    case IPOPL:
    case IDUMP:
    case IRMMOVL:
      return TRUE;

    default:
      return FALSE;
  }
}

unsigned int getStatus(bool memError, bool validIns,
    unsigned int icode)
{ 
  if (memError)
  {
    return SADR;
  }
  else if(!validIns)
  {
    return SINS;
  }
  else if(icode == IHALT)
  {
    return SHLT;
  }
  else
  {
    return SAOK;
  }
}

bool needRegByte(unsigned int icode)
{
  switch (icode)
  {
    case IRRMOVL:
    case IOPL:
    case IPUSHL:
    case IPOPL:
    case IIRMOVL:
    case IRMMOVL:
    case IMRMOVL:
      return TRUE;

    default:
      return FALSE;
  }
}

unsigned int predictPC(unsigned int pc, unsigned int icode,
    unsigned int valC)
{
  if (icode == IJXX || icode == ICALL)
  {
    return valC;
  }
  else
  {
    return getValP(pc, icode);
  }
}

unsigned int getValP(unsigned int pc, unsigned int icode)
{
  switch (icode)
  {
    // 2 byte instructions (opcode + reg)
    case IRRMOVL:
    case IOPL:
    //case ICMOVXX: // duplicates IRRMOVL
    case IPUSHL:
    case IPOPL:
      return pc + 2;

    // 5 byte instructions (opcode + valc)
    case IJXX:
    case ICALL:
    case IDUMP:
      return pc + 5;

    // 6 byte instructions (opcode + reg + valc)
    case IIRMOVL:
    case IRMMOVL:
    case IMRMOVL:
      return pc + 6;

    // 1 byte instructions (opcode)
    case IHALT:
    case INOP:
    case IRET:
    default:
      return pc + 1;
  }
}

unsigned int getRA(unsigned int icode, unsigned char byte)
{
  return getBits(HIGH_BIT_START, HIGH_BIT_END, byte);
}

unsigned int getRB(unsigned int icode, unsigned char byte)
{
  return getBits(LOW_BIT_START, LOW_BIT_END, byte);
}

bool needValC(unsigned int icode)
{
  switch (icode)
  {
    case IIRMOVL:
    case IRMMOVL:
    case IMRMOVL:
    case IJXX:
    case ICALL:
    case IDUMP:
      return TRUE;

    default:
      return FALSE;
  }
}

unsigned int getValC(unsigned int pc, unsigned int icode,
    bool* memError)
{
  assert(memError != NULL && "Missing memError");

  // set the location of valC relative to the pc
  unsigned int pcOffset = 1;
  if (icode == IIRMOVL || icode == IRMMOVL || icode == IMRMOVL)
  {
    ++pcOffset;
  }

  unsigned char bytes[4];
  for (int i = 0; i < 4; ++i)
  {
    bytes[i] = getByte(pc + pcOffset + i, memError);
  }

  return *memError ? 0 :
    buildWord(bytes[0], bytes[1], bytes[2], bytes[3]);
}

void fetchStage(pipelineForward pf)
{
  // tracks any errors accessing memory this stage
  bool memError = FALSE;
  
  // get the pc and the opcode
  unsigned int pc = fetchPC(pf);
  unsigned char byte = getByte(pc, &memError);
  unsigned int icode;
  unsigned int ifun;  
  // set appropriate values on a memory error
  if (memError)
  {
    icode = INOP;
    ifun = FNONE;
  }
  else
  {
    icode = getIcode(byte);
    ifun = getIfun(byte);
  }

  // validate the icode and set the status
  bool isValid = validInstruction(icode);
  unsigned int stat = getStatus(memError, isValid, icode);

  // get register values, if needed
  unsigned int rA = RNONE;
  unsigned int rB = RNONE;
  if (!memError && needRegByte(icode))
  {
    byte = getByte(pc + 1, &memError);
    rA = getRA(icode, byte);
    rB = getRB(icode, byte);
  }

  // get valC, if needed
  unsigned int valC = 0;
  if (!memError && needValC(icode))
  {
    valC = getValC(pc, icode, &memError);
  }

  // get the next value of the pc
  unsigned int valP = getValP(pc, icode);
  F.predPC = predictPC(pc, icode, valC);

  // update the decode stage and F register
  updateDregister(stat, icode, ifun, rA, rB, valC, valP);
}

fregister getFregister()
{
    return F;
}

void clearFregister()
{
    clearBuffer((char *) &F, sizeof(F));
}
