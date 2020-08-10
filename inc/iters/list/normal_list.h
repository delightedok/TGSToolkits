#ifndef __ITERS_LIST_NORMAL_LIST_H_
#define __ITERS_LIST_NORMAL_LIST_H_

#include <iters/list/list_comms.h>

/**
 *
 * note: without locker
 *
 **/
class NormalListObject: public ListObject
{
public:
    TGSTK_EXPORT NormalListObject(ListVTable & mVTable);
    TGSTK_EXPORT int iterate(Func_itersListOnIterate onIter, void * arg, int unwind = 0);
    TGSTK_EXPORT int rpush(void * obj, int size);
    TGSTK_EXPORT int lpush(void * obj, int size);
    TGSTK_EXPORT void * rpop(void);
    TGSTK_EXPORT void * lpop(void);
    TGSTK_EXPORT int clear(void);
private:
protected:
};

#endif
