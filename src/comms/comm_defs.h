#ifndef __COMMS_COMM_DEFS_H_
#define __COMMS_COMM_DEFS_H_

#ifndef COMM_ASSERT_RETURN
#define COMM_ASSERT_RETURN(cond, retVal) do { if (!(cond)) return retVal; } while (0)
#endif

#ifndef COMM_LOCK
#define COMM_LOCK(L) (L).lock()
#endif

#ifndef COMM_UNLOCK
#define COMM_UNLOCK(L) (L).unlock()
#endif

#endif