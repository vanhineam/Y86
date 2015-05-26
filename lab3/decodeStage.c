#include "bool.h"
#include "tools.h"
#include "decodeStage.h"

/// D holds the inputs for the decode stage.
static dregister D;

dregister getDregister()
{
  return D;
}

void clearDregister()
{
  clearBuffer((char *) &D, sizeof(D));
}
