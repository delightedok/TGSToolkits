#include "../../comms/comm_headers.h"
#include <sorts/quick_sort.h>

#define THIS_FILE "quick_sort.cpp"
#define LOG_TAG "SORTS-QUICK"

TGSTK_EXPORT SortQuickObject::SortQuickObject(SortVTable & vTable) : SortObject(vTable)
{}

int SortQuickObject::_sort(void * objs, int elemSize, int size, SortType type, int begin, int end, void * tempObj)
{
    COMM_ASSERT_RETURN(objs && size > 0 && size >= end && begin >= 0, -1);

    int ret = 0;
    int i = begin, j = end;
    int rc = 0;
    void * base = tempObj;
    do {
        if (begin >= end)  // only one element should be handled
        {
            break;
        }

        if (!tempObj)  // NOT alloc memory outside the function, then alloc it in the function
        {
            base = this->onDuplicate(COMM_ARRAY_ELEM(objs, elemSize, i));
            if (!base)
            {
                mlog_e(LOG_TAG, THIS_FILE, "Failed to duplicate element[%d].", i);
                ret = -1;
                break;
            }
        } else
        {
            this->onCopy(base, COMM_ARRAY_ELEM(objs, elemSize, i));
        }

        switch (type)
        {
            case emSortDesc:
            case emSortAsc:
                {}
                break;
            default:
                {
                    ret = -1;
                    mlog_e(LOG_TAG, THIS_FILE, "Invalid Param[type]: %d\n", type);
                }
                break;
        }

        while (i < j && !ret)
        {
            while (i < j)  // end to begin
            {
                rc = this->onCompare(base, COMM_ARRAY_ELEM(objs, elemSize, j));
                if ((emSortDesc == type && rc < 0) || (emSortAsc == type && rc > 0))
                {
                    break;
                }
                j--;
            }
            if (i < j)
            {
                this->onCopy(COMM_ARRAY_ELEM(objs, elemSize, i), COMM_ARRAY_ELEM(objs, elemSize, j));
                i++;
            }

            rc = this->onCompare(base, COMM_ARRAY_ELEM(objs, elemSize, i));
            while (i < j)  // begin to end
            {
                rc = this->onCompare(base, COMM_ARRAY_ELEM(objs, elemSize, i));
                if ((emSortDesc == type && rc > 0) || (emSortAsc == type && rc < 0))
                {
                    break;
                }
                i++;
            }
            if (i < j)
            {
                this->onCopy(COMM_ARRAY_ELEM(objs, elemSize, j), COMM_ARRAY_ELEM(objs, elemSize, i));
                j--;
            }
        }
        if (i >= j)  // in fact, if 0 == ret, then i == j
        {
            this->onCopy(COMM_ARRAY_ELEM(objs, elemSize, j), base);
        }
        if (i >= j)
        {
            ret = this->_sort(objs, elemSize, size, type, begin, i - 1, base);
            if (ret)
            {
                break;
            }
            ret = this->_sort(objs, elemSize, size, type, i + 1, end, base);
        }
        if (!tempObj)  // Alloc memory in this function, then the free operation should be processed
        {
            this->onFree(base);
        }
    } while (0);
    return ret;
}

int SortQuickObject::onSort(void * objs, int elemSize, int size, SortType type)
{
    return _sort(objs, elemSize, size, type, 0, size - 1, NULL);
}

