#ifndef WRITEBACKSTAGE_H
#define WRITEBACKSTAGE_H

#include "bool.h"

typedef struct
{
  unsigned int stat, icode, valE, valM, dstE, dstM;
} wregister;

/**
 * Sets the values of the M register values 
 */
void updateWregister(unsigned int stat, unsigned int icode,
    unsigned int valE, unsigned int valA, unsigned int dstE,
    unsigned int dstM);
/**
 * Performs the write back stage.
 * @return true if execution should stop.
 */
extern bool writebackStage();

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
