#include "../comms/comm_headers.h"
#include <sorts/sort_comms.h>

using namespace std;

SortStatistic::SortStatistic(void)
{
    this->reset();
}

string SortStatistic::get_statistic(void)
{
    string ret("");
    ret += "Allocs: " + to_string(this->mNAllocs) + " ";
    ret += "Exchanges: " + to_string(this->mNExchanges) + " ";
    ret += "Compares: " + to_string(this->mNCompares);
    return ret;
}

void SortStatistic::reset(void)
{
    this->mNExchanges = 0;
    this->mNAllocs = 0;
    this->mNCompares = 0;
}

void SortStatistic::addExchange(void)
{
    this->mNExchanges++;
}

void SortStatistic::addAlloc(void)
{
    this->mNAllocs++;
}

void SortStatistic::addCompare(void)
{
    this->mNCompares++;
}

SortObject::SortObject(SortVTable & vTable)
{
    memcpy(&this->mVTable, &vTable, sizeof(SortVTable));
}

int SortObject::onCompare(void * obj1, void * obj2)
{
    int ret = -1;
    this->mStatistic.addCompare();
    ret = this->mVTable.onCompare(obj1, obj2);
    return ret;
}

int SortObject::onExchange(void * obj1, void * obj2)
{
    int ret = -1;
    this->mStatistic.addExchange();
    ret = this->mVTable.onExchange(obj1, obj2);
    return ret;
}

void * SortObject::onDuplicate(void * obj)
{
    void * ret = NULL;
    this->mStatistic.addAlloc();
    ret = this->mVTable.onDuplicate(obj);
    return ret;
}

void SortObject::onFree(void * obj)
{
    this->mVTable.onFree(obj);
}

int SortObject::sort(void * objs, int elemSize, int size, SortType type)
{
    this->mStatistic.reset();
    return 0;
}

TGSTK_EXPORT string SortObject::get_statistic(void)
{
    return this->mStatistic.get_statistic();
}