#include "bool.h"
#include "tools.h"
#include "writebackStage.h"

//W register holds the input for the writeback stage.
static wregister W;

wregister getWregister()
{
  return W;
}

void clearWregister()
{
  clearBuffer((char *) &W, sizeof(W));
}
