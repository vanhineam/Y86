#ifndef FETCHSTAGE_H
#define FETCHSTAGE_H

typedef struct 
{
    unsigned int predPC;
} fregister;

/**
 * Performs the fetch stage.
 */
extern void fetchStage();

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
