#ifndef MEMORYSTAGE_H
#define MEMORYSTAGE_H

#include "forwarding.h"

typedef struct
{
  unsigned int stat, icode, Cnd, valE, valA, dstE, dstM;
} mregister;

/**
 * Updates the M register with the provided values.
 */
void updateMregister(unsigned int stat, unsigned int icode,
    unsigned int Cnd, unsigned int valE, unsigned int valA,
    unsigned int dstE, unsigned int dstM);

/**
 * Performs the memory stage.
 */
extern void memoryStage(pipelineForward* forward);

/**
 * Returns a copy of the M register
 * @return M
 */
extern mregister getMregister();

/**
 * Clears the input of the memory stage
 */
extern void clearMregister();

#endif
