#include "../../comms/comm_headers.h"
#include <sorts/bubble_sort.h>

#define THIS_FILE "bubble_sort.cpp"
#define LOG_TAG "SORTS-BUBBLE"

TGSTK_EXPORT SortBubbleObject::SortBubbleObject(SortVTable & vTable) : SortObject(vTable)
{}

int SortBubbleObject::onSort(void * objs, int elemSize, int size, SortType type)
{
    COMM_ASSERT_RETURN(objs && size > 0, -1);

    int ret = 0;
    int i = 0, j = 0;
    int rc = 0;
    for (i = 1; i < size && !ret; i++)
    {
        for (j = 1; j < size - i + 1 && !ret; j++)
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
    return ret;
}
