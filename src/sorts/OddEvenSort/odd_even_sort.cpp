#include "../../comms/comm_headers.h"
#include <sorts/odd_even_sort.h>

#define THIS_FILE "odd_even_sort.cpp"
#define LOG_TAG "SORTS-ODD-EVEN"

TGSTK_EXPORT SortOddEvenObject::SortOddEvenObject(SortVTable & vTable) : SortObject(vTable)
{}

int SortOddEvenObject::onSort(void * objs, int elemSize, int size, SortType type)
{
    COMM_ASSERT_RETURN(objs && size > 0, -1);

    int ret = 0;
    int rc = 0;
    int i = 0;
    int odd = 1;
    int hasExchange = 1;
    while (!ret && hasExchange)
    {
        hasExchange = 0;
        if (odd)  // compare elements in odd index and even index
        {
            for (i = 1; i < size && !ret; i += 2)
            {
                rc = this->onCompare(COMM_ARRAY_ELEM(objs, elemSize, i - 1), COMM_ARRAY_ELEM(objs, elemSize, i));
                switch (type)
                {
                    case emSortDesc:
                        {
                            if (rc < 0)
                            {
                                ret = this->onExchange(
                                    COMM_ARRAY_ELEM(objs, elemSize, i - 1),
                                    COMM_ARRAY_ELEM(objs, elemSize, i));
                                hasExchange = 1;
                            }
                        }
                        break;
                    case emSortAsc:
                        {
                            if (rc > 0)
                            {
                                ret = this->onExchange(
                                    COMM_ARRAY_ELEM(objs, elemSize, i - 1),
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
            odd = 0;
        } else  // compare elements in even index and (even + 1) index
        {
            for (i = 2; i < size && !ret; i += 2)
            {
                rc = this->onCompare(COMM_ARRAY_ELEM(objs, elemSize, i - 1), COMM_ARRAY_ELEM(objs, elemSize, i));
                switch (type)
                {
                    case emSortDesc:
                        {
                            if (rc < 0)
                            {
                                ret = this->onExchange(
                                    COMM_ARRAY_ELEM(objs, elemSize, i - 1),
                                    COMM_ARRAY_ELEM(objs, elemSize, i));
                                hasExchange = 1;
                            }
                        }
                        break;
                    case emSortAsc:
                        {
                            if (rc > 0)
                            {
                                ret = this->onExchange(
                                    COMM_ARRAY_ELEM(objs, elemSize, i - 1),
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
            odd = 1;
        }
    }
    return ret;
}

