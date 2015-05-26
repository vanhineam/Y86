#include "decodeStage.h"

#include "bool.h"
#include "tools.h"
#include "executeStage.h"
#include "registers.h"
#include "instructions.h"

#include <assert.h>

/// D holds the inputs for the decode stage.
static dregister D;

/**
 * Gets the srcA register based on the icode in the D register.
 * @return the register value.
 */
static unsigned int getSrcA();

/**
 * Gets the srcB register based on the icode in the D register.
 * @return the register value.
 */
static unsigned int getSrcB();

/**
 * Gets the dstE register based on the icode in the D register.
 * @return the register value.
 */
static unsigned int getDstE();

/**
 * Gets the dstM register based on the icode in the D register.
 * @return the register value.
 */
static unsigned int getDstM();

/**
 * Forwards into the decode stage for valA
 * @return appropriate valA based on the instruction 
 */
static unsigned int getValA(unsigned int srcA, pipelineForward* forward);

/**
 * Forwards into the decode stage for valB
 * @return appropriate valB based on the instruction
 */
static unsigned int getValB(unsigned int srcB, pipelineForward* forward);

unsigned int getSrcA()
{
  switch (D.icode)
  {
    case IRRMOVL:
    case IRMMOVL:
    case IOPL:
    case IPUSHL:
      return D.rA;

    case IPOPL:
    case IRET:
      return ESP;

    default:
      return RNONE;
  }
}

unsigned int getSrcB()
{
  switch (D.icode)
  {
    case IOPL:
    case IRMMOVL:
    case IMRMOVL:
      return D.rB;

    case IPUSHL:
    case IPOPL:
    case ICALL:
    case IRET:
      return ESP;

    default:
      return RNONE;
  }
}

unsigned int getDstE()
{
  switch (D.icode)
  {
    case IRRMOVL:
    case IIRMOVL:
    case IOPL:
      return D.rB;

    case IPUSHL:
    case IPOPL:
    case ICALL:
    case IRET:
      return ESP;

    default:
      return RNONE;
  }
}

unsigned int getDstM()
{
  switch (D.icode)
  {
    case IMRMOVL:
    case IPOPL:
      return D.rA;

    default:
      return RNONE;
  }
}

unsigned int getValA(unsigned int srcA, pipelineForward* forward)
{
  if (srcA == RNONE)
  {
    return 0;
  }
  else if(srcA == forward->W_dstE)
  {
    return forward->W_valE;
  }
  else
  {
    return getRegister(srcA);
  }
}

unsigned int getValB(unsigned int srcB, pipelineForward* forward)
{
  if (srcB == RNONE)
  {
    return 0;
  }
  else if(srcB == forward->W_dstE)
  {
    return forward->W_valE;
  }
  else
  {
    return getRegister(srcB);
  }
}

void updateDregister(unsigned int stat, unsigned int icode,
    unsigned int ifun, unsigned int rA, unsigned int rB,
    unsigned int valC, unsigned int valP)
{
  D.stat = stat;
  D.icode = icode;
  D.ifun = ifun;
  D.rA = rA;
  D.rB = rB;
  D.valC = valC;
  D.valP = valP;
}

void decodeStage(pipelineForward forward)
{
  unsigned int srcA = getSrcA();
  unsigned int srcB = getSrcB();
  unsigned int dstE = getDstE();
  unsigned int dstM = getDstM();
  unsigned int valA = getValA(srcA ,&forward);
  unsigned int valB = getValB(srcB, &forward);

  updateEregister(D.stat, D.icode, D.ifun, D.valC, valA, valB,
      dstE, dstM, srcA, srcB);
}


dregister getDregister()
{
  return D;
}

void clearDregister()
{
  clearBuffer((char *) &D, sizeof(D));
}
