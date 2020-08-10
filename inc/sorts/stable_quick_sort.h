#ifndef __SORT_STABLE_QUICK_SORT_H_
#define __SORT_STABLE_QUICK_SORT_H_

#include <tgstk_comms.h>
#include <sorts/sort_comms.h>

class SortStableQuickObject : public SortObject
{
public:
    TGSTK_EXPORT SortStableQuickObject(SortVTable & vTable);
private:
    int _sort(void * objs, int elemSize, int size, SortType type, int begin, int end, void * tempObj);
private:
    static int on_sort_list_iterate(void * element, void * arg);
protected:
    int onSort(void * objs, int elemSize, int size, SortType type);
};

#endif
