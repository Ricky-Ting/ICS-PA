#ifndef __COMMON_H__
#define __COMMON_H__

/* Uncomment these macros to enable corresponding functionality. */
#define HAS_CTE
//#define HAS_VME

#include <am.h>
#include <klib.h>
#include "debug.h"

#define off_t size_t
#define ssize_t size_t

typedef char bool;
#define true 1
#define false 0

#endif
