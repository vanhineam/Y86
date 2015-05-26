#include "bool.h"
#include "tools.h"
#include "executeStage.h"

//E Register holds the input for the execute stage.
static eregister E;

eregister getEregister()
{
  return E;
}

void clearEregister()
{
  clearBuffer((char *) &E, (sizeof(E)));
}

