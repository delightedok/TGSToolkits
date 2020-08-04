#ifndef __SORT_COCKTAIL_SHAKER_SORT_H_
#define __SORT_COCKTAIL_SHAKER_SORT_H_

#include <tgstk_comms.h>
#include <sorts/sort_comms.h>

class SortCocktailShakerObject : public SortObject
{
public:
    TGSTK_EXPORT SortCocktailShakerObject(SortVTable & vTable);
    TGSTK_EXPORT int sort(void * objs, int elemSize, int size, SortType type);
private:
protected:
};

#endif
