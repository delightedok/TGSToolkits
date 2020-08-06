#ifndef __SORT_SORT_COMMS_H_
#define __SORT_SORT_COMMS_H_

#include <string>

typedef int (* Func_sort1)(void * obj1, void * obj2);
typedef void (* Func_sort2)(void * dst, void * src);
typedef void * (* Func_sort3)(void * obj);
typedef void (* Func_sort4)(void * obj);

class SortStatistic
{
public:
    SortStatistic(void);
    std::string get_statistic(void);
    void reset(void);
    void addExchange(void);
    void addCopy(void);
    void addAlloc(void);
    void addCompare(void);
private:
    long long mNExchanges;  // The times of exchange
    long long mNAllocs;  // The times of alloc memory
    long long mNCompares;  // The times of alloc memory
    long long mNCopys;  // The times of copy elements
};

struct SortVTable
{
    Func_sort1 onCompare;  // compare 2 objects
    Func_sort1 onExchange;  // exchange 2 objects
    Func_sort2 onCopy;  // copy from src to dst
    Func_sort3 onDuplicate;  // duplicate an object
    Func_sort4 onFree;  // free an object
};

enum SortType
{
    emSortDesc = 0,  // Descending
    emSortAsc        // Ascending
};

class SortObject
{
public:
    TGSTK_EXPORT int sort(void * objs, int elemSize, int size, SortType type);
    TGSTK_EXPORT std::string get_statistic(void);  // Get the display string of statistic
protected:
    SortObject(SortVTable & vTable);
    int onCompare(void * obj1, void * obj2);
    int onExchange(void * obj1, void * obj2);
    void onCopy(void * dst, void * src);
    void * onDuplicate(void * obj);
    void onFree(void * obj);
    virtual int onSort(void * objs, int elemSize, int size, SortType type);
private:
    SortStatistic mStatistic;
    SortVTable mVTable;
};

#endif