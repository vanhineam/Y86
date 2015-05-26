#ifndef FETCHSTAGE_H
#define FETCHSTAGE_H

typedef struct 
{
    unsigned int predPC;
} fregister;

/**
 * Returns a copy of the F register.
 * @return the F register.
 */
fregister getFregister();

/**
 * Clears the F register.
 */
void clearFregister();

#endif
