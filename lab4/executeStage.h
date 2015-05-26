#ifndef EXECUTESTAGE_H
#define EXECUTESTAGE_H
typedef struct
{
    unsigned int stat, icode, ifun, valC, valA, valB, dstE, dstM, srcA, srcB;
} eregister;

/**
 * Returns a copy of the E register
 * @return E
 */
eregister getEregister();

/**
 * Clears the execute register
 */
void clearEregister();
#endif
