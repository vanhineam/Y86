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
#include "debug.h"
#include "forwarding.h"
#include "status.h"
#include <stdio.h>

/**
 * Initializes the processor for usage.
 */
static void initialize();

int main(int argc, char * argv[])
{
  argc -= setDebugOptions(argc, argv);

  initialize();

  bool loadError = !load(argc, argv);
  if (loadError)
  {
    dumpMemory();
    return 1;
  }

  unsigned int clockCount = 0;
  bool stop = FALSE;
  
  pipelineForward forward;
  statusType stat;

  clearBuffer((char*)&forward, sizeof(pipelineForward));
  clearBuffer((char*)&stat, sizeof(stat));

  while(!stop)
  {
    dumpStage(DBG_WRITEBACK_STAGE);
    stop = writebackStage(&forward, &stat);

    dumpStage(DBG_MEMORY_STAGE);
    memoryStage(&forward, &stat);

    dumpStage(DBG_EXECUTE_STAGE);
    executeStage(&forward, stat);

    dumpStage(DBG_DECODE_STAGE);
    decodeStage(forward);

    dumpStage(DBG_FETCH_STAGE);
    fetchStage(forward);

    ++clockCount;
  }

  printf("\nTotal clock cycles = %d\n", clockCount);

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


