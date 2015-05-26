#ifndef FETCHSTAGE_H
#define FETCHSTAGE_H

#include "forwarding.h"

typedef struct 
{
    unsigned int predPC;
} fregister;

/**
 * Performs the fetch stage.
 * @param pf the pipeline forwarding values.
 */
extern void fetchStage(pipelineForward pf);

/**
 * Returns a copy of the F register.
 * @return the F register.
 */
extern fregister getFregister();

/**
 * Clears the F register.
 */
extern void clearFregister();

#endif
