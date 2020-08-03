#include "../comms/comm_headers.h"
#include <sorts/sort_comms.h>

SortStatistic::SortStatistic(void)
{}

void SortStatistic::display(void)
{}

SortObject::SortObject(SortVTable & vTable)
{
    memcpy(&this->mVTable, &vTable, sizeof(SortVTable));
}

int SortObject::onCompare(void * obj1, void * obj2)
{
    int ret = -1;
    ret = this->mVTable.onCompare(obj1, obj2);
    return ret;
}

int SortObject::onExchange(void * obj1, void * obj2)
{
    int ret = -1;
    ret = this->mVTable.onExchange(obj1, obj2);
    return ret;
}

void * SortObject::onDuplicate(void * obj)
{
    void * ret = NULL;
    ret = this->mVTable.onDuplicate(obj);
    return ret;
}

void SortObject::onFree(void * obj)
{
    this->mVTable.onFree(obj);
}

void SortObject::statistic_display(void)
{}