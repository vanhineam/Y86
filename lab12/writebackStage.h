#ifndef WRITEBACKSTAGE_H
#define WRITEBACKSTAGE_H

#include "bool.h"
#include "status.h"
#include "forwarding.h"

typedef struct
{
  unsigned int stat, icode, valE, valM, dstE, dstM;
} wregister;

/**
 * Sets the values of the M register values 
 */
extern void updateWregister(unsigned int stat, unsigned int icode,
    unsigned int valE, unsigned int valM, unsigned int dstE,
    unsigned int dstM);
/**
 * Performs the write back stage.
 * @param forward the values forwarded from later in the pipeline.
 * @param state the pipeline status values.
 * @return true if execution should stop.
 */
extern bool writebackStage(pipelineForward* forward, statusType* stat);

/**
 * Returns a copy of the W register
 * @return W
 */
extern wregister getWregister();

/**
 * Clears the input values for the writeback stage
 */
extern void clearWregister();

#endif
