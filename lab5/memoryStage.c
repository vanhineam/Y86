#include "memoryStage.h"

#include "bool.h"
#include "tools.h"
#include "writebackStage.h"

//M register holds the input for the memory stage.
static mregister M;

void memoryStage()
{
  updateWregister(M.stat, M.icode, M.valE, M.valA, M.dstE, M.dstM);
}

void updateMregister(unsigned int stat, unsigned int icode,
    unsigned int Cnd, unsigned int valE, unsigned int valA,
    unsigned int dstE, unsigned int dstM)
{
  M.stat = stat;
  M.icode = icode;
  M.Cnd = Cnd;
  M.valE = valE;
  M.valA = valA;
  M.dstE = dstE;
  M.dstM = dstM;
}

mregister getMregister()
{
  return M;
}

void clearMregister()
{
  clearBuffer((char *) &M, sizeof(M));
}
