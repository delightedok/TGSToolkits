#ifndef __LIST_COMMS_H_
#define __LIST_COMMS_H_

#include <tgstk_comms.h>

typedef int (* Func_itersListOnIterate)(void * element);

class ListObject
{
public:
    virtual int iterate(Func_itersListOnIterate onIter, void * arg) = 0;
private:
protected:
};

#endif