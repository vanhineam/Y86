#ifndef DEBUG_H
#define DEBUG_H

#define DBG_FETCH_STAGE      0
#define DBG_DECODE_STAGE     1
#define DBG_EXECUTE_STAGE    2
#define DBG_MEMORY_STAGE     3
#define DBG_WRITEBACK_STAGE  4

/**
 * Sets the debug options from the command line.
 * @param argc the number of command line args.
 * @param argv the value of the command line args.
 * @return the number debug options parsed from the command line.
 */
extern int setDebugOptions(int argc, char** argv);

/**
 * Dumps the register of the specified stage to the console,
 * if output for that stage is enabled.
 * @param stage the stage to be dumped.
 */
extern void dumpStage(int stage);

/**
 * Prints out debug information if messages are enabled for the specified
 * stage.
 * @param stage the targetted output stage.
 * @param str a printf style format string.
 * @param ... any parameters necessary for the format string.
 */
extern void debugPrint(int stage, char* str, ...);

#define PRINT_FETCH(str, ...) \
  debugPrint(DBG_FETCH_STAGE, str, __VA_ARGS___)
#define PRINT_DECODE(str, ...) \
  debugPrint(DBG_DECODE_STAGE, str, __VA_ARGS__)
#define PRINT_EXECUTE(str, ...) \
  debugPrint(DBG_EXECUTE_STAGE, str, __VA_ARGS__)
#define PRINT_MEMORY(str, ...) \
  debugPrint(DBG_MEMORY_STAGE, str, __VA_ARGS__)
#define PRINT_WRITEBACK(str, ...) \
  debugPrint(DBG_WRITEBACK_STAGE, str, __VA_ARGS__)

#endif
