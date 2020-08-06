#ifndef __SORT_OPTIMIZED_GNOME_SORT_H_
#define __SORT_OPTIMIZED_GNOME_SORT_H_

#include <tgstk_comms.h>
#include <sorts/sort_comms.h>

class SortOptimizedGnomeObject : public SortObject
{
public:
    TGSTK_EXPORT SortOptimizedGnomeObject(SortVTable & vTable);
private:
protected:
    TGSTK_DEPRECATED int onSort1(void * objs, int elemSize, int size, SortType type);
    int onSort(void * objs, int elemSize, int size, SortType type);  // optimize the exchange
};

#endif
