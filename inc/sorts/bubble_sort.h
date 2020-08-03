#ifndef __SORT_BUBBLE_SORT_H_
#define __SORT_BUBBLE_SORT_H_

#include <sorts/sort_comms.h>

class SortBubbleObject : public SortObject
{
public:
    SortBubbleObject(SortVTable & vTable);
    int sort(void * objs, int elemSize, int size, SortType type);
private:
protected:
};

#endif