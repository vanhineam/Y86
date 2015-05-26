#include "memoryStage.h"

#include "bool.h"
#include "tools.h"
#include "writebackStage.h"
#include "instructions.h"
#include "status.h"
#include "memory.h"
#include "registers.h"

//M register holds the input for the memory stage.
static mregister M;

/**
 *  Selects the memory address
 */
static unsigned int mem_addr();

/**
 * Set the read control signal
 */
static bool mem_read();

/**
 * Set the write control signal
 */
static bool mem_write();

/*
 * Control the reading and writing to and from memory
 */
static bool mem_control(unsigned int address, bool read, bool write, unsigned int* value);

void memoryStage(pipelineForward* forward, statusType* status)
{
  unsigned int valM = M.valA;
  unsigned int address = mem_addr();
  bool memError = mem_control(address, mem_read(), mem_write(), &valM);

  unsigned int stat = memError ? SADR : M.stat;

  forward->M_valE = M.valE;
  forward->M_valA = M.valA;
  forward->M_dstM = M.dstM;
  forward->M_dstE = M.dstE;
  forward->m_valM = valM;
  forward->M_Cnd = M.Cnd;
  forward->M_icode = M.icode;

  status->m_stat = stat;

  updateWregister(stat, M.icode, M.valE, valM, M.dstE, M.dstM);
}

unsigned int mem_addr()
{
  switch(M.icode)
  {
    case IRMMOVL:
    case IPUSHL:
    case ICALL:
    case IMRMOVL:
      return M.valE;
    case IPOPL:
    case IRET:
      return M.valA;

    default:
      return 0;
  }
}

bool mem_read()
{
  return M.icode == IMRMOVL || M.icode == IPOPL || M.icode == IRET;
}

bool mem_write()
{
  return M.icode == IRMMOVL || M.icode == IPUSHL || M.icode == ICALL;
}

bool mem_control(unsigned int address, bool read, bool write, unsigned int* value)
{
  bool memError = FALSE;

  if(read)
  {
    *value = getWord(address, &memError);
  }
  if(write)
  {
    putWord(address, *value, &memError);
  }
  return memError;
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
  M.stat = SAOK;
  M.icode = INOP;
  M.dstE = RNONE;
  M.dstM = RNONE;
}
