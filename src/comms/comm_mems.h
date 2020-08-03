#ifndef __COMMS_COMM_MEMS_H_
#define __COMMS_COMM_MEMS_H_

void * _comm_mems_alloc(unsigned int size);

void _comm_mems_free(void ** p);

#ifndef mmalloc
#define mmalloc(size) _comm_mems_alloc(size)
#endif

#ifndef mfree
#define mfree(p) _comm_mems_free(&(p))
#endif

#endif