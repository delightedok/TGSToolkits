#ifndef __SORT_ODD_EVEN_SORT_H_
#define __SORT_ODD_EVEN_SORT_H_

#include <tgstk_comms.h>
#include <sorts/sort_comms.h>

class SortOddEvenObject : public SortObject
{
public:
    TGSTK_EXPORT SortOddEvenObject(SortVTable & vTable);
private:
protected:
    int onSort(void * objs, int elemSize, int size, SortType type);
};

#endif
