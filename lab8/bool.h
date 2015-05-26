#ifndef BOOL_H
#define BOOL_H

#if defined( __STDC_VERSION) && (__STDC_VERSION__ == 199901L)

#include <stdbool.h>

#define TRUE true
#define FALSE false
// C99 defines a bool type

#else

#define TRUE 1
#define FALSE 0
typedef int bool;

#endif

#endif
