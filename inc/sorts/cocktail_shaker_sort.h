#ifndef __SORT_COCKTAIL_SHAKER_SORT_H_
#define __SORT_COCKTAIL_SHAKER_SORT_H_

#include <tgstk_comms.h>
#include <sorts/sort_comms.h>

class SortCocktailShakerObject : public SortObject
{
public:
    TGSTK_EXPORT SortCocktailShakerObject(SortVTable & vTable);
private:
protected:
    int onSort(void * objs, int elemSize, int size, SortType type);
};

#endif
