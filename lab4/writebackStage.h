#ifndef WRITEBACKSTAGE_H
#define WRITEBACKSTAGE_H

typedef struct
{
  unsigned int stat, icode, valE, valM, dstE, dstM;
} wregister;

/**
 * Returns a copy of the W register
 * @return W
 */
wregister getWregister();
/**
 * Clears the input values for the writeback stage
 */
void clearWregister();

#endif
