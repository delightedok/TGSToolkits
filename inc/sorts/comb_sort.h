#ifndef __SORT_COMB_SORT_H_
#define __SORT_COMB_SORT_H_

#include <tgstk_comms.h>
#include <sorts/sort_comms.h>

class SortCombObject : public SortObject
{
public:
    TGSTK_EXPORT SortCombObject(SortVTable & vTable, float factor = 1.3F);
private:
    float factor;
protected:
    int onSort(void * objs, int elemSize, int size, SortType type);
};

#endif
