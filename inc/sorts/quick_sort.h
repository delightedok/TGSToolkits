#ifndef __SORT_QUICK_SORT_H_
#define __SORT_QUICK_SORT_H_

#include <tgstk_comms.h>
#include <sorts/sort_comms.h>

class SortQuickObject : public SortObject
{
public:
    TGSTK_EXPORT SortQuickObject(SortVTable & vTable);
private:
    int _sort(void * objs, int elemSize, int size, SortType type, int begin, int end);
protected:
    int onSort(void * objs, int elemSize, int size, SortType type);
};

#endif

