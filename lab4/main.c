#include <stdio.h>
#include "bool.h"
#include "tools.h"
#include "memory.h"
#include "dump.h"
#include "loader.h"
#include "decodeStage.h"
#include "executeStage.h"
#include "fetchStage.h"
#include "memoryStage.h"
#include "writebackStage.h"
#include "registers.h"

void initialize();

int main(int argc, char * argv[])
{
  initialize();

  bool loadError = !load(argc, argv);
  //if (loadError)
  //{
  //  return 1;
  //}

  dumpProgramRegisters();
  dumpProcessorRegisters();
  dumpMemory();

  return 0;
}

void initialize()
{
    clearMemory();
    clearRegisters();
    clearFregister();
    clearDregister();
    clearEregister();
    clearMregister();
    clearWregister();
}


