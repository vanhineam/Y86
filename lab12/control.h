#ifndef CONTROL_H
#define CONTROL_H

/**
 * Defines a collection of values used in pipeline control for bubbling
 * and stalling.
 */
typedef struct
{
  unsigned int d_srcA;
  unsigned int d_srcB;
  unsigned int D_icode;
  
  unsigned int E_icode;
  unsigned int E_dstM;
  unsigned int e_Cnd;

  unsigned int M_icode;
} control;

#endif
