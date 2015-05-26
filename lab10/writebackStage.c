#include "writebackStage.h"

#include "tools.h"
#include "tools.h"
#include "dump.h"
#include "instructions.h"
#include "registers.h"
#include "status.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define PROGRAMBIT 0
#define PROCESSORBIT 1
#define MEMORYBIT 2

//W register holds the input for the writeback stage.
static wregister W;

/**
 * Dumps either the Program, Processor, or Memory Registers based on 
 * W.valE
 */
static void dump();

bool writebackStage(pipelineForward* forward, statusType* stat)
{
  assert(forward != NULL && "forward is null");

  if (W.icode == IDUMP)
  {
    dump();
  }

  if (W.stat == SAOK)
  {
    setRegister(W.dstE, W.valE);
    setRegister(W.dstM, W.valM);
  }
  
  forward->W_valE = W.valE;
  forward->W_dstE = W.dstE;
  forward->W_valM = W.valM;
  forward->W_dstM = W.dstM;
  
  stat->W_stat = W.stat;

  if(W.stat == SINS)
  {
    printf("Invalid instruction\n");
    dumpProgramRegisters();
    dumpProcessorRegisters();
    dumpMemory();
  }
  else if(W.stat == SADR)
  {
    printf("Invalid memory address\n");
    dumpProgramRegisters();
    dumpProcessorRegisters();
    dumpMemory();
  }

  return W.stat == SHLT || W.stat == SADR || W.stat == SINS;
}

void dump()
{
  bool programRegisters = getBits(PROGRAMBIT, PROGRAMBIT, W.valE);
  bool processorRegisters = getBits(PROCESSORBIT, PROCESSORBIT, W.valE);
  bool memoryRegisters = getBits(MEMORYBIT, MEMORYBIT, W.valE);

  if(programRegisters)
  {
    dumpProgramRegisters();
  }
  if(processorRegisters)
  {
    dumpProcessorRegisters();
  }
  if(memoryRegisters)
  {
    dumpMemory();
  }
}

void updateWregister(unsigned int stat, unsigned int icode,
    unsigned int valE, unsigned int valM, unsigned int dstE,
    unsigned int dstM)
{
  W.stat = stat;
  W.icode = icode;
  W.valE = valE;
  W.valM = valM;
  W.dstE = dstE;
  W.dstM = dstM;
}

wregister getWregister()
{
  return W;
}

void clearWregister()
{
  clearBuffer((char *) &W, sizeof(W));
  W.stat = SAOK;
  W.icode = INOP;
}
