#ifndef __SORT_DUAL_PIVOT_QUICK_SORT_H_
#define __SORT_DUAL_PIVOT_QUICK_SORT_H_

#include <tgstk_comms.h>
#include <sorts/sort_comms.h>

class SortDualPivotQuickObject : public SortObject
{
public:
    TGSTK_EXPORT SortDualPivotQuickObject(SortVTable & vTable);
private:
    int _sort(void * objs, int elemSize, int size, SortType type,
        int begin, int end, void * tempObj1, void * tempObj2);
protected:
    int onSort(void * objs, int elemSize, int size, SortType type);
};

#endif

