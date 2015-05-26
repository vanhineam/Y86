#include "executeStage.h"

#include "bool.h"
#include "tools.h"
#include "instructions.h"
#include "memoryStage.h"
#include "writebackStage.h"
#include "registers.h"
#include "debug.h"

//E Register holds the input for the execute stage.
static eregister E;

// pointers to the instruction functions
static InstructionFunction instructions[16];

/*
 *
 * Instruction stub which does nothing.
 */
static unsigned int doNothing();

/**
 * Performs the dump instruciton.
 */
static unsigned int insDump();

/**
 * Performs the opl instruction.
 */
static unsigned int insOpl();

/**
 * Performs the irmovl instruction.
 */
static unsigned int insIrmovl();

/**
 * Performs the cmovxx family of instructions.
 */
static unsigned int insCmovxx();

/**
 * Performs the mrmovl instructions.
 */
static unsigned int insMrmovl();

/**
 * Performs the rmmovl instruction.
 */
static unsigned int insRmmovl();

/**
 * Gets the ALU input value A.
 */
static unsigned int getAluA();

/**
 * Gets the ALU input value B.
 */
static unsigned int getAluB();

/**
 * Gets the function to be executed by the ALU.
 */
static unsigned int getAluFun();

/**
 * Executes the ALU.
 */
static unsigned int executeAlu();

/**
 * Determines if the condition codes should be updated.
 */
static bool needToSetCC();

/**
 * Set thes condition code.
 */
static unsigned int getCnd();

/**
 * Gets dstE.
 */
static unsigned int getDstE(unsigned int cnd);

unsigned int doNothing()
{
  return 0;
}

unsigned int insDump()
{
  return E.valC;
}

unsigned int insOpl()
{
  return executeAlu();
}

unsigned int insIrmovl()
{
  return E.valC;
}

unsigned int insCmovxx()
{
  return E.valA;
}

unsigned int insMrmovl()
{
  return executeAlu();
}

unsigned int insRmmovl()
{
  return executeAlu();
}

unsigned int getAluA()
{
  switch (E.icode)
  {
    case IRRMOVL:
    case IOPL:
      return E.valA;

    case IIRMOVL:
    case IRMMOVL:
    case IMRMOVL:
    case IDUMP:
      return E.valC;

    case ICALL:
    case IPUSHL:
      return -4;

    case IRET:
    case IPOPL:
      return 4;

    default:
      return 0;
  }
}

unsigned int getAluB()
{
  switch (E.icode)
  {
    case IRMMOVL:
    case IMRMOVL:
    case IOPL:
    case ICALL:
    case IPUSHL:
    case IRET:
    case IPOPL:
      return E.valB;

    case IRRMOVL:
    case IIRMOVL:
    default:
      return 0;
  }
}

unsigned int getAluFun()
{
  if (E.icode == IOPL)
  {
    return E.ifun;
  }
  else
  {
    return FADDL;
  }
}

unsigned int executeAlu(statusType* stat)
{
  unsigned int a = getAluA();
  unsigned int b = getAluB();
  unsigned int fun = getAluFun();
  unsigned int result = 0;

  switch (fun)
  {
    case FADDL:
      result = a + b;
      break;

    case FSUBL:
      result = b - a;
      break;

    case FANDL:
      result = a & b;
      break;

    case FXORL:
      result = a ^ b;
      break;
  }

  if (needToSetCC(stat))
  {
    unsigned int isZero = result == 0;
    unsigned int isSigned = isNegative(result);
    unsigned int isOverflow = 0;

    if (fun == FADDL)
    {
      isOverflow = (isNegative(a) == isNegative(b)) &&
        (isNegative(result) != isNegative(a));
    }
    else if (fun == FSUBL)
    {
      isOverflow = (isNegative(a) != isNegative(b)) &&
        (isNegative(result) == isNegative(a));
    }

    setCC(ZF, isZero);
    setCC(SF, isSigned);
    setCC(OF, isOverflow);
  }

  return result;
}

bool needToSetCC(statusType* stat)
{
  return E.icode == IOPL &&
    !(stat->m_stat == SADR || stat->m_stat == SINS || stat->m_stat == SHLT) &&
    !(stat->W_stat == SADR || stat->W_stat == SINS || stat->W_stat == SHLT);
}

unsigned int getCnd()
{
  if (E.icode == ICMOVXX)
  {
    switch (E.ifun)
    {
      case FCMOVLE:
        return (getCC(ZF) || getCC(SF)) || getCC(OF);

      case FCMOVL:
        return getCC(SF) || getCC(OF);

      case FCMOVE:
        return getCC(ZF);

      case FCMOVNE:
        return !getCC(ZF);

      case FCMOVGE:
        return (getCC(ZF) || !getCC(SF)) || getCC(OF);

      case FCMOVG:
        return (!getCC(ZF) && !getCC(SF)) || getCC(OF);

      case FRRMOVL:
        return 1;
    }
  }

  return 0;
}

unsigned int getDstE(unsigned int cnd)
{
  if (E.icode == IRRMOVL && !cnd)
  {
    return RNONE;
  }
  else
  {
    return E.dstE;
  }
}

void initInstructions()
{
  for (int i = 0; i < 16; ++i)
  {
    instructions[i] = doNothing;
  }

  instructions[IOPL] = insOpl;
  instructions[ICMOVXX] = insCmovxx;
  instructions[IIRMOVL] = insIrmovl;
  instructions[IMRMOVL] = insMrmovl;
  instructions[IRMMOVL] = insRmmovl;
  instructions[IDUMP] = insDump;
}

void executeStage(pipelineForward* forward, statusType* stat)
{
  // DR NORRIS SAID THIS IS OK
  unsigned int valE = executeAlu(stat); //instructions[E.icode]();
  unsigned int Cnd = getCnd();
  unsigned int dstE = getDstE(Cnd);

  forward->e_valE = valE;
  forward->e_dstE = dstE;

  bool M_bubble = FALSE;

  switch(stat->m_stat)
  {
    case SADR:
    case SINS:
    case SHLT:
      M_bubble = TRUE;
  }
  
  switch(stat->W_stat)
  {
    case SADR:
    case SINS:
    case SHLT:
      M_bubble = TRUE;
  }

  if(M_bubble)
  {
    clearMregister();
  }
  else
  {
    updateMregister(E.stat, E.icode, Cnd, valE, E.valA, dstE, E.dstM);  
  }
}

void updateEregister(unsigned int stat, unsigned int icode,
       unsigned int ifun, unsigned int valC, unsigned int valA,
       unsigned int valB, unsigned int dstE, unsigned int dstM,
       unsigned int srcA, unsigned int srcB)
{
  E.stat = stat;
  E.icode = icode;
  E.ifun = ifun;
  E.valC = valC;
  E.valA = valA;
  E.valB = valB;
  E.dstE = dstE;
  E.dstM = dstM;
  E.srcA = srcA;
  E.srcB = srcB;
}

eregister getEregister()
{
  return E;
}

void clearEregister()
{
  clearBuffer((char *) &E, (sizeof(E)));
  E.stat = SAOK;
  E.icode = INOP;
}

