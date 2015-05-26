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

//F register holds the input for the fetch stage. 
static fregister F;

// defines the boundaries of the low and high bits of a byte
const int lowBits[2] = { 0, 3 };
const int highBits[2] = { 4, 7 };

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
static unsigned int fetchPC();

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
 * Predicts the next value of the pc based on the icode.
 * @param pc the current value of the pc.
 * @param icode the current icode.
 * @return the predicted value of the pc.
 */
static unsigned int predictPC(unsigned int pc, unsigned int icode);

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

unsigned int fetchPC()
{
  mregister M = getMregister();
  wregister W = getWregister();

  // branch misprediction after jump
  if (M.icode == IJXX && !M.Cnd)
  {
    return M.valA;
  }

  // completion of RET
  if (W.icode == IRET)
  {
    return W.valM;
  }

  // use default predicted pc
  return F.predPC;
}

unsigned int getIcode(unsigned char byte)
{
  return getBits(highBits[0], highBits[1], byte);
}

unsigned int getIfun(unsigned char byte)
{
  return getBits(lowBits[0], lowBits[1], byte);
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
    case IIRMOVL:
    case IRMMOVL:
    case IMRMOVL:
    case IJXX:
    case ICALL:
      return TRUE;

    default:
      return FALSE;
  }
}

unsigned int predictPC(unsigned int pc, unsigned int icode)
{
  unsigned int mod = 0;

  switch (icode)
  {
    // 1 byte instructions (opcode)
    case IHALT:
    case INOP:
    case IRET:
      mod += 1;
      break;

    // 2 byte instructions (opcode + reg)
    case IRRMOVL:
    case IOPL:
    //case ICMOVXX: // duplicates IRRMOVL
    case IPUSHL:
    case IPOPL:
      mod += 2;
      break;

    // 5 byte instructions (opcode + valc)
    case IJXX:
    case ICALL:
    case IDUMP:
      mod += 5;
      break;

    // 6 byte instructions (opcode + reg + valc)
    case IIRMOVL:
    case IRMMOVL:
    case IMRMOVL:
      mod += 6;
  }

  return pc + mod;
}

unsigned int getRA(unsigned int icode, unsigned char byte)
{
  if (icode == IIRMOVL)
  {
    const unsigned int irmovlRA = 0x8;
    return irmovlRA;
  }
  else
  {
    return getBits(highBits[0], highBits[1], byte);
  }
}

unsigned int getRB(unsigned int icode, unsigned char byte)
{
  if (icode == IPUSHL || icode == IPOPL)
  {
    const unsigned int pushAndPopRA = 0xf;
    return pushAndPopRA;
  }
  else
  {
    return getBits(lowBits[0], lowBits[1], byte);
  }
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

void fetchStage()
{
  // tracks any errors accessing memory this stage
  bool memError = FALSE;
  
  // get the pc and the opcode
  unsigned int pc = fetchPC();
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
  unsigned int valP = predictPC(pc, icode);

  // update the decode stage and F register
  updateDregister(stat, icode, ifun, rA, rB, valC, valP);
  F.predPC = valP;
}

fregister getFregister()
{
    return F;
}

void clearFregister()
{
    clearBuffer((char *) &F, sizeof(F));
}
