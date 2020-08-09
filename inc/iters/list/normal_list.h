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
    TGSTK_EXPORT int iterate(Func_itersListOnIterate onIter, void * arg);
private:
protected:
};

#endif
