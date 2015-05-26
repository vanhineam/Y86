#ifndef MEMORYSTAGE_H
#define MEMORYSTAGE_H
typedef struct
{
  unsigned int stat, icode, Cnd, valE, valA, dstE, dstM;
} mregister;

/**
 * Returns a copy of the M register
 * @return M
 */
mregister getMregister();

/**
 * Clears the input of the memory stage
 */
void clearMregister();
#endif
