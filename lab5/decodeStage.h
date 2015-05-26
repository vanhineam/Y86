#ifndef DECODESTAGE_H
#define DECODESTAGE_H

typedef struct
{
    unsigned int stat, icode, ifun, rA, rB, valC, valP;
} dregister;

/**
 * Updates the Dregister to the values passed.
 */
extern void updateDregister(unsigned int stat, unsigned int icode,
    unsigned int ifun, unsigned int rA, unsigned int rB,
    unsigned int valC, unsigned int valP);

/**
 * Peforms the decode stage.
 */
extern void decodeStage();

/** 
 * Retuns a copy of the decode register
 * @return the decode register
 */
extern dregister getDregister();

/**
 * Clears the decode register values
 */
extern void clearDregister();

#endif
