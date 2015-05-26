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

/**
 * @return the ALU input value A.
 */
static unsigned int getAluA();

/**
 * @return the ALU input value B.
 */
static unsigned int getAluB();

/**
 * @return the function to be executed by the ALU.
 */
static unsigned int getAluFun();

/**
 * Executes the ALU.
 * @return the output value from the ALU.
 */
static unsigned int executeAlu();

/**
 * @return if the condition codes should be updated.
 */
static bool needToSetCC();

/**
 * @return the condition for jumps and conditional moves.
 */
static unsigned int getCnd();

/**
 * @return the destination of the ALU value.
 */
static unsigned int getDstE(unsigned int cnd);

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

unsigned int executeAlu(statusType stat)
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

bool needToSetCC(statusType stat)
{
  return E.icode == IOPL &&
    !(stat.m_stat == SADR || stat.m_stat == SINS || stat.m_stat == SHLT) &&
    !(stat.W_stat == SADR || stat.W_stat == SINS || stat.W_stat == SHLT);
}

unsigned int getCnd()
{
  if (E.icode == ICMOVXX || E.icode == IJXX)
  {
    bool zero = getCC(ZF);
    bool sign = getCC(SF);
    bool overflow = getCC(OF);

    switch (E.ifun)
    {
      case F_LE:
        return ((zero || sign) && !overflow) ||
               (overflow && !sign);

      case F_L:
        return (sign && !(zero || overflow)) ||
               (overflow && !sign);

      case F_E:
        return zero;

      case F_NE:
        return !zero;

      case F_GE:
        return (!overflow && (zero || !sign)) ||
               (overflow && !zero && sign);

      case F_G:
        return (!overflow && !sign && !zero) ||
               (overflow && sign);

      case F_NO_CND:
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

void executeStage(pipelineForward* forward, statusType stat, control* con)
{
  // DR NORRIS SAID THIS IS OK
  unsigned int valE = executeAlu(stat); 
  unsigned int Cnd = getCnd();
  unsigned int dstE = getDstE(Cnd);

  forward->e_valE = valE;
  forward->e_dstE = dstE;

  con->E_icode = E.icode;
  con->E_dstM = E.dstM;
  con->e_Cnd = Cnd;

  // determines if the memory stage should be bubbled
  bool M_bubble = 
    (stat.m_stat == SADR || stat.m_stat == SINS || stat.m_stat == SHLT) ||
    (stat.W_stat == SADR || stat.W_stat == SINS || stat.W_stat == SHLT);

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
  E.srcA = RNONE;
  E.srcB = RNONE;
  E.dstE = RNONE;
  E.dstM = RNONE;
}

