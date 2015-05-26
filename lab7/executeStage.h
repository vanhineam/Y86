#ifndef EXECUTESTAGE_H
#define EXECUTESTAGE_H

typedef struct
{
    unsigned int stat, icode, ifun, valC, valA, valB, dstE, dstM, srcA, srcB;
} eregister;

/**
 * Initializes instructions to be executed in this stage.
 */
extern void initInstructions();

/**
 * Performs the execute stage.
 */
extern void executeStage();

/**
 * Updates the E register with the provided values.
 */
extern void updateEregister(unsigned int stat, unsigned int icode,
    unsigned int ifun, unsigned int valC, unsigned int valA,
    unsigned int valB, unsigned int dstE, unsigned int dstM,
    unsigned int srcA, unsigned int srcB);

/**
 * Returns a copy of the E register
 * @return E
 */
extern eregister getEregister();

/**
 * Clears the execute register
 */
extern void clearEregister();

#endif
