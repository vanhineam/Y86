#ifndef EXECUTESTAGE_H
#define EXECUTESTAGE_H

#include "forwarding.h"
#include "status.h"
#include "control.h"

typedef struct
{
    unsigned int stat, icode, ifun, valC, valA, valB, dstE, dstM, srcA, srcB;
} eregister;

/**
 * Performs the execute stage.
 * @param forward the pipeline forwarding information.
 * @param stat the pipeline status information.
 */
extern void executeStage(pipelineForward* forward, statusType stat, control* con);

/**
 * Updates the E register with the provided values.
 */
extern void updateEregister(unsigned int stat, unsigned int icode,
    unsigned int ifun, unsigned int valC, unsigned int valA,
    unsigned int valB, unsigned int dstE, unsigned int dstM,
    unsigned int srcA, unsigned int srcB);

/**
 * Returns a copy of the E register
 * @return the E register
 */
extern eregister getEregister();

/**
 * Clears the execute register to a NOP instruction, bubbling the stage.
 */
extern void clearEregister();

#endif
