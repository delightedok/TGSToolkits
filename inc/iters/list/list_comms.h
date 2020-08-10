#ifndef __LIST_COMMS_H_
#define __LIST_COMMS_H_

#include <tgstk_comms.h>
#include <comms/comms_types.h>

typedef int (* Func_itersListOnIterate)(void * element, void * arg);

typedef void * (* Func_itersList1)(void * obj);
typedef void (* Func_itersList2)(void * obj);

struct ListNode
{
    PointerT ptr;
    ListNode * prev;
    ListNode * next;
};

struct ListVTable
{
    Func_itersList1 onDuplicate;
    Func_itersList2 onFree;
};

class ListObject
{
public:
    ListObject(ListVTable & mVTable);
    virtual int iterate(Func_itersListOnIterate onIter, void * arg, int unwind = 0) = 0;
    virtual int rpush(void * obj, int size) = 0;
    virtual int lpush(void * obj, int size) = 0;
    virtual void * rpop(void) = 0;
    virtual void * lpop(void) = 0;
    virtual int clear(void) = 0;
    TGSTK_EXPORT virtual int size(void);
protected:
    void * doDuplicate(void * obj);
    void doFree(void * obj);
private:
    ListVTable mVTable;
protected:
    ListNode mDatas;
    int mNdatas;
};

#endif