#ifndef DECODESTAGE_H
#define DECODESTAGE_H

typedef struct
{
    unsigned int stat, icode, ifun, rA, rB, valC, valP;
} dregister;

/** 
 * Retuns a copy of the decode register
 * @return the decode register
 */
dregister getDregister();

/**
 * Clears the decode register values
 */
void clearDregister();
#endif
