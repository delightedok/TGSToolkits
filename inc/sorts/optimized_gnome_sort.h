#ifndef __SORT_GNOME_SORT_H_
#define __SORT_GNOME_SORT_H_

#include <tgstk_comms.h>
#include <sorts/sort_comms.h>

class SortOptimizedGnomeObject : public SortObject
{
public:
    TGSTK_EXPORT SortOptimizedGnomeObject(SortVTable & vTable);
    TGSTK_EXPORT int sort(void * objs, int elemSize, int size, SortType type);
    TGSTK_EXPORT int sort2(void * objs, int elemSize, int size, SortType type);  // optimize the exchange
private:
protected:
};

#endif
