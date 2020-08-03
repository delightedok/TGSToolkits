#ifndef __SORT_BUBBLE_SORT_H_
#define __SORT_BUBBLE_SORT_H_

#include <tgstk_comms.h>
#include <sorts/sort_comms.h>

class SortBubbleObject : public SortObject
{
public:
    TGSTK_EXPORT SortBubbleObject(SortVTable & vTable);
    TGSTK_EXPORT int sort(void * objs, int elemSize, int size, SortType type);
private:
protected:
};

#endif