#include "bool.h"
#include "tools.h"
#include "memoryStage.h"

//M register holds the input for the memory stage.
static mregister M;

mregister getMregister()
{
  return M;
}

void clearMregister()
{
  clearBuffer((char *) &M, sizeof(M));
}
