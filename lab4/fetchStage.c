#include "bool.h"
#include "tools.h"
#include "fetchStage.h"

//F register holds the input for the fetch stage. 
static fregister F;

fregister getFregister()
{
    return F;
}

void clearFregister()
{
    clearBuffer((char *) &F, sizeof(F));
}
