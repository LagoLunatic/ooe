#ifndef GLOBAL_H
#define GLOBAL_H

#include "fx_types.h"
#include "types.h"

#define GLUE(a, b) a##b
#define GLUE2(a, b) GLUE(a, b)
#define STATIC_ASSERT(cond) typedef char GLUE2(static_assertion_failed, __LINE__)[(cond) ? 1 : -1]

#endif /* GLOBAL_H */
