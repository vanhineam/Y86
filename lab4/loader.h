#ifndef LOADER_H
#define LOADER_H

#include "bool.h"

/**
 * Loads a machine instructions file specified by the command line, 
 * validating it and loading it into memory.
 * @param argc the number of command line arguments.
 * @param argv the command line arguments.
 * @return true if the file was loaded successfully.
 */
bool load(int argc, char** argv);

#endif
