#ifndef FORWARDING_H
#define FORWARDING_H

typedef struct
{
  unsigned int W_valE;
  unsigned int W_dstE;
  unsigned int W_valM;
  unsigned int W_dstM;

  unsigned int e_dstE;
  unsigned int e_valE;

  unsigned int m_valM;
  unsigned int M_valE;
  unsigned int M_dstM;
  unsigned int M_dstE;
  unsigned int M_valA;
} pipelineForward;

#endif
