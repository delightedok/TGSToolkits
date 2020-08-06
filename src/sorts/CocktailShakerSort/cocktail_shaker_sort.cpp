#include "../../comms/comm_headers.h"
#include <sorts/cocktail_shaker_sort.h>

#define THIS_FILE "cocktail_shaker_sort.cpp"
#define LOG_TAG "SORTS-COCKTAIL_SHAKER"

TGSTK_EXPORT SortCocktailShakerObject::SortCocktailShakerObject(SortVTable & vTable) : SortObject(vTable)
{}

int SortCocktailShakerObject::onSort(void * objs, int elemSize, int size, SortType type)
{
    COMM_ASSERT_RETURN(objs && size > 0, -1);

    int ret = 0;
    int i = 0, j = 0;
    int rc = 0;
    SortObject::sort(objs, elemSize, size, type);
    for (i = 1; i < size && !ret; i++)
    {
        if (i % 2)  // odd
        {
            for (j = (i / 2 + 1); j < (size - i / 2) && !ret; j++)
            {
                rc = this->onCompare(COMM_ARRAY_ELEM(objs, elemSize, j - 1), COMM_ARRAY_ELEM(objs, elemSize, j));
                switch (type)
                {
                    case emSortDesc:
                        {
                            if (rc < 0)
                            {
                                ret = this->onExchange(
                                    COMM_ARRAY_ELEM(objs, elemSize, j - 1),
                                    COMM_ARRAY_ELEM(objs, elemSize, j));
                            }
                        }
                        break;
                    case emSortAsc:
                        {
                            if (rc > 0)
                            {
                                ret = this->onExchange(
                                    COMM_ARRAY_ELEM(objs, elemSize, j - 1),
                                    COMM_ARRAY_ELEM(objs, elemSize, j));
                            }
                        }
                        break;
                    default:
                        {
                            ret = -1;
                            mlog_e(LOG_TAG, THIS_FILE, "Invalid Param[type]: %d\n", type);
                        }
                        break;
                }
            }
        } else  // even
        {
            for (j = (size - i / 2 - 1); j >= i / 2 && !ret; j--)
            {
                rc = this->onCompare(COMM_ARRAY_ELEM(objs, elemSize, j - 1), COMM_ARRAY_ELEM(objs, elemSize, j));
                switch (type)
                {
                    case emSortDesc:
                        {
                            if (rc < 0)
                            {
                                ret = this->onExchange(
                                    COMM_ARRAY_ELEM(objs, elemSize, j - 1),
                                    COMM_ARRAY_ELEM(objs, elemSize, j));
                            }
                        }
                        break;
                    case emSortAsc:
                        {
                            if (rc > 0)
                            {
                                ret = this->onExchange(
                                    COMM_ARRAY_ELEM(objs, elemSize, j - 1),
                                    COMM_ARRAY_ELEM(objs, elemSize, j));
                            }
                        }
                        break;
                    default:
                        {
                            ret = -1;
                            mlog_e(LOG_TAG, THIS_FILE, "Invalid Param[type]: %d\n", type);
                        }
                        break;
                }
            }
        }
    }
    return ret;
}

