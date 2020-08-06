#include "../../comms/comm_headers.h"
#include <sorts/comb_sort.h>

#define THIS_FILE "comb_sort.cpp"
#define LOG_TAG "SORTS-COMB"

TGSTK_EXPORT SortCombObject::SortCombObject(SortVTable & vTable, float factor) : SortObject(vTable)
{
    this->factor = factor;
    if (factor <= 1)
    {
        mlog_e(LOG_TAG, THIS_FILE, "Param[factor](%d) should be larger than 1. Now set to 1.3F.", factor);
        this->factor = 1.3F;
    }
}

int SortCombObject::onSort(void * objs, int elemSize, int size, SortType type)
{
    COMM_ASSERT_RETURN(objs && size > 0, -1);

    int ret = 0;
    int rc = 0;
    int i = 0;
    int hasExchange = 0;
    int gap = size;
    while (gap > 1 || hasExchange)
    {
        hasExchange = 0;
        gap = gap > 1 ? (int)((float)gap / this->factor) : gap;
        for (i = gap; i < size; i++)
        {
            rc = this->onCompare(COMM_ARRAY_ELEM(objs, elemSize, i - gap), COMM_ARRAY_ELEM(objs, elemSize, i));
            switch (type)
            {
                case emSortDesc:
                    {
                        if (rc < 0)
                        {
                            this->onExchange(
                                COMM_ARRAY_ELEM(objs, elemSize, i - gap),
                                COMM_ARRAY_ELEM(objs, elemSize, i));
                            hasExchange = 1;
                        }
                    }
                    break;
                case emSortAsc:
                    {
                        if (rc > 0)
                        {
                            this->onExchange(
                                COMM_ARRAY_ELEM(objs, elemSize, i - gap),
                                COMM_ARRAY_ELEM(objs, elemSize, i));
                            hasExchange = 1;
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

