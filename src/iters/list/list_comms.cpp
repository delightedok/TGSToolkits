#include "../../comms/comm_headers.h"
#include <iters/list/list_comms.h>

using namespace std;

void * iters_list_on_duplicate_default(void * obj)
{
    return obj;
}

void iters_list_on_free_default(void * obj)
{
    COMM_UNUSED(obj);
}

ListObject::ListObject(ListVTable & mVTable)
{
    memset(&this->mDatas, 0, sizeof(this->mDatas));
    this->mNdatas = 0;
    memcpy(&this->mVTable, &mVTable, sizeof(ListVTable));
    COMM_ASSIGN_IF(this->mVTable.onDuplicate, iters_list_on_duplicate_default, !mVTable.onDuplicate);
    COMM_ASSIGN_IF(this->mVTable.onFree, iters_list_on_free_default, !mVTable.onFree);
}

void * ListObject::doDuplicate(void * obj)
{
    return this->mVTable.onDuplicate(obj);
}

void ListObject::doFree(void * obj)
{
    this->mVTable.onFree(obj);
}

