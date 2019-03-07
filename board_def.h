
// #define T4_V12 
#define T4_V13
// #define T10_V18

#if defined (T10_V18)
#include "T10_V18.h"
#elif defined(T4_V12)
#include "T4_V12.h"
#elif defined(T4_V13)
#include "T4_V13.h"
#else
#error "please select board version"
#endif


