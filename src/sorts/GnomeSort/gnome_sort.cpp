#include "../../comms/comm_headers.h"
#include <sorts/gnome_sort.h>

#define THIS_FILE "gnome_sort.cpp"
#define LOG_TAG "SORTS-GNOME"

TGSTK_EXPORT SortGnomeObject::SortGnomeObject(SortVTable & vTable) : SortObject(vTable)
{}

TGSTK_EXPORT int SortGnomeObject::sort(void * objs, int elemSize, int size, SortType type)
{
    COMM_ASSERT_RETURN(objs && size > 0, -1);

    int ret = 0;
    int i = 0;
    int rc = 0;

    SortObject::sort(objs, elemSize, size, type);

    for (i = 1; i < size && !ret;)
    {
        rc = this->onCompare(COMM_ARRAY_ELEM(objs, elemSize, i - 1), COMM_ARRAY_ELEM(objs, elemSize, i));
        switch (type)
        {
            case emSortDesc:
                {
                    if (rc >= 0)
                    {
                        i++;
                    } else
                    {
                        ret = this->onExchange(COMM_ARRAY_ELEM(objs, elemSize, i - 1),
                            COMM_ARRAY_ELEM(objs, elemSize, i));
                        i--;
                    }
                }
                break;
            case emSortAsc:
                {
                    if (rc <= 0)
                    {
                        i++;
                    } else
                    {
                        ret = this->onExchange(COMM_ARRAY_ELEM(objs, elemSize, i - 1),
                            COMM_ARRAY_ELEM(objs, elemSize, i));
                        i--;
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
    return ret;
}

