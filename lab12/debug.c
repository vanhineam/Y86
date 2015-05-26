#include "debug.h"

#include "bool.h"
#include "fetchStage.h"
#include "decodeStage.h"
#include "executeStage.h"
#include "memoryStage.h"
#include "writebackStage.h"

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>

#define NUM_DBG_OPTS 5

static bool debugOptions[NUM_DBG_OPTS] = {
  FALSE, FALSE, FALSE, FALSE, FALSE
};

// command line args
static const char* argHelp = "--help";
//static const char* argDump = "-d=";

static const char* help =
  "Usage: yess <filename.yo> [options]\n"
  "Options:\n"
  " \"--help\": This help text.\n"
  " \"-d=stage1[,stages...]\": Print the values for stage registers at the\n"
  "                            beginning of the stage.\n"
  "                            all = print all stages\n"
  "                            fetch = print fetch stage\n"
  "                            decode = print decode stage\n"
  "                            execute = print execute stage\n"
  "                            memory = print memory stage\n"
  "                            writeback = print writeback stage";

static const char* dumpAll = "all";
static const char* dumpFetch = "fetch";
static const char* dumpDecode = "decode";
static const char* dumpExecute = "execute";
static const char* dumpMemory = "memory";
static const char* dumpWriteback = "writeback";

int setDebugOptions(int argc, char** argv)
{
  int result = 0;

  for (int i = 0; i < argc; ++i)
  {
    // look for the start of an arg
    if (argv[i][0] == '-')
    {
      // start on the second char of the option
      char* opt = argv[i] + 1;

      // look for full word args
      if (*opt == '-')
      {
        if (strcmp(argHelp, argv[i]) == 0)
        {
          printf("%s\n", help);
          ++result;
        }
      }
      else if (*opt == 'd' && *(opt + 1) == '=')
      {
        // push the pointer past the equals
        opt += 2;
        char* stage = strtok(opt, ",");
        while (stage != NULL)
        {
          if (strcmp(stage, dumpAll) == 0)
          {
            for (int i = 0; i < NUM_DBG_OPTS; ++i)
            {
              debugOptions[i] = TRUE;
            }
          }
          else if (strcmp(stage, dumpFetch) == 0)
          {
            debugOptions[DBG_FETCH_STAGE] = TRUE;
          }
          else if (strcmp(stage, dumpDecode) == 0)
          {
            debugOptions[DBG_DECODE_STAGE] = TRUE;
          }
          else if (strcmp(stage, dumpExecute) == 0)
          {
            debugOptions[DBG_EXECUTE_STAGE] = TRUE;
          }
          else if (strcmp(stage, dumpMemory) == 0)
          {
            debugOptions[DBG_MEMORY_STAGE] = TRUE;
          }
          else if (strcmp(stage, dumpWriteback) == 0)
          {
            debugOptions[DBG_WRITEBACK_STAGE] = TRUE;
          }

          stage = strtok(NULL, ",");
        }

        ++result;
      }
    }
  }

  return result;
}

void dumpStage(int stage)
{
#ifdef NDEBUG
  assert(stage < NUM_DBG_OPTS && "Invalid stage");

  if (!debugOptions[stage])
  {
    return;
  }
  
  switch (stage)
  {
    case DBG_FETCH_STAGE:
      {
        fregister F = getFregister();
        printf("***FETCH STAGE DUMP***\n");
        printf("\tpredPC: %08x\n", F.predPC);
      }
      break;

    case DBG_DECODE_STAGE:
      {
        dregister D = getDregister();
        printf("***DECODE STAGE DUMP***\n");
        printf("\tstat: %u\n", D.stat);
        printf("\ticode: %#x\n", D.icode);
        printf("\tifun: %#x\n", D.ifun);
        printf("\trA: %08x\n", D.rA);
        printf("\trB: %08x\n", D.rB);
        printf("\tvalC: %d\n", D.valC);
        printf("\tvalP: %08x\n", D.valP);
      }
      break;

    case DBG_EXECUTE_STAGE:
      {
        eregister E = getEregister();
        printf("***EXECUTE STAGE DUMP***\n");
        printf("\tstat: %u\n", E.stat);
        printf("\ticode: %#x\n", E.icode);
        printf("\tifun: %#x\n", E.ifun);
        printf("\tvalC: %d\n", E.valC);
        printf("\tvalA: %d\n", E.valA);
        printf("\tvalB: %d\n", E.valB);
        printf("\tdstE: %#x\n", E.dstE);
        printf("\tdstM: %#x\n", E.dstM);
        printf("\tsrcA: %#x\n", E.srcA);
        printf("\tsrcB: %#x\n", E.srcB);
      }
      break;

    case DBG_MEMORY_STAGE:
      {
        mregister M = getMregister();
        printf("***MEMORY STAGE DUMP***\n");
        printf("\tstat: %u\n", M.stat);
        printf("\ticode: %#x\n", M.icode);
        printf("\tCnd: %#x\n", M.Cnd);
        printf("\tvalE: %d\n", M.valE);
        printf("\tvalA: %#x\n", M.valA);
        printf("\tdstE: %#x\n", M.dstE);
        printf("\tdstM: %#x\n", M.dstM);
      }
      break;

    case DBG_WRITEBACK_STAGE:
      {
        wregister W = getWregister();
        printf("***WRITEBACK STAGE DUMP***\n");
        printf("\tstat: %u\n", W.stat);
        printf("\ticode: %#x\n", W.icode);
        printf("\tvalE: %d\n", W.valE);
        printf("\tvalM: %d\n", W.valM);
        printf("\tdstE: %#x\n", W.dstE);
        printf("\tdstM: %#x\n", W.dstM);
      }
      break;
  }
#endif
}

void debugPrint(int stage, char* str, ...)
{
#ifdef NDEBUG
  assert(stage < NUM_DBG_OPTS && "Invalid stage");

  if (debugOptions[stage])
  {
    va_list args;
    va_start(args, str);
    vprintf(str, args);
    va_end(args);
  }
#endif
}
