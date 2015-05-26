#ifndef FETCHSTAGE_H
#define FETCHSTAGE_H

#include "forwarding.h"
#include "control.h"

typedef struct 
{
    unsigned int predPC;
} fregister;

/**
 * Performs the fetch stage.
 * @param pf the pipeline forwarding values.
 * @param con the pipeline control values.
 */
extern void fetchStage(pipelineForward pf, control con);

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
