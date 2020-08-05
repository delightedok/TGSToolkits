#ifndef __SORT_GNOME_SORT_H_
#define __SORT_GNOME_SORT_H_

#include <tgstk_comms.h>
#include <sorts/sort_comms.h>

class SortGnomeObject : public SortObject
{
public:
    TGSTK_EXPORT SortGnomeObject(SortVTable & vTable);
    TGSTK_EXPORT int sort(void * objs, int elemSize, int size, SortType type);
private:
protected:
};

#endif
