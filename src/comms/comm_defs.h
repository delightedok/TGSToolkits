#ifndef __COMMS_COMM_DEFS_H_
#define __COMMS_COMM_DEFS_H_

#include <tgstk_comms.h>

#ifndef COMM_ASSERT_RETURN
#define COMM_ASSERT_RETURN(cond, retVal) do { if (!(cond)) return retVal; } while (0)
#endif

#ifndef COMM_LOCK
#define COMM_LOCK(L) (L).lock()
#endif

#ifndef COMM_UNLOCK
#define COMM_UNLOCK(L) (L).unlock()
#endif

#ifndef COMM_ARRAY_ELEM
#define COMM_ARRAY_ELEM(array, elemSize, i) (((char *)(array)) + (elemSize) * (i))
#endif

#ifndef COMM_ASSIGN_IF
#define COMM_ASSIGN_IF(dst, src, cond) do { if (cond) dst = src; } while (0)
#endif

#ifndef COMM_UNUSED
#define COMM_UNUSED(param) (void)(param)
#endif

#endif