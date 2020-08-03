#ifndef __SORT_SORT_COMMS_H_
#define __SORT_SORT_COMMS_H_

typedef int (* Func_sort1)(void * obj1, void * obj2);
typedef int (* Func_sort2)(void * obj1, void * obj2);
typedef void * (* Func_sort3)(void * obj);
typedef void (* Func_sort4)(void * obj);

class SortStatistic
{
public:
    SortStatistic(void);
    void display(void);
private:
};

struct SortVTable
{
    Func_sort1 onCompare;  // compare 2 objects
    Func_sort2 onExchange;  // exchange 2 objects
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
    virtual int sort(void * objs, int elemSize, int size, SortType type) = 0;
    void statistic_display(void);  // display the statistic
protected:
    SortObject(SortVTable & vTable);
    int onCompare(void * obj1, void * obj2);
    int onExchange(void * obj1, void * obj2);
    void * onDuplicate(void * obj);
    void onFree(void * obj);
private:
    SortStatistic mStatistic;
    SortVTable mVTable;
};

#endif