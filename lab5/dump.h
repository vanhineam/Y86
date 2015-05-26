#ifndef DUMP_H
#define DUMP_H

/**
 * Outputs the contents of the yess little endian memory in WORDSPERLINE four
 * byte words per line.  A * is displayed a the end of a line if each line in
 * memory after that up to the next line displayed is identical to the * line.
 */
extern void dumpMemory();

/**
 * Outputs the contents of the YESS program registers to stdout.
 */
extern void dumpProgramRegisters();

/**
 * Outputs the contents of the YESS processor registers to stdout.
 */
extern void dumpProcessorRegisters();

#endif
