#include "../../comms/comm_headers.h"
#include <sorts/quick_sort.h>

#define THIS_FILE "quick_sort.cpp"
#define LOG_TAG "SORTS-QUICK"

TGSTK_EXPORT SortQuickObject::SortQuickObject(SortVTable & vTable) : SortObject(vTable)
{}

int SortQuickObject::_sort(void * objs, int elemSize, int size, SortType type, int begin, int end)
{
    COMM_ASSERT_RETURN(objs && size > 0 && size >= end && begin >= 0, -1);

    int ret = 0;
    int i = begin, j = end;
    int rc = 0;
    void * base = NULL;
    do {
        if (begin >= end)  // only one element should be handled
        {
            break;
        }

        base = this->onDuplicate(COMM_ARRAY_ELEM(objs, elemSize, i));
        if (!base)
        {
            mlog_e(LOG_TAG, THIS_FILE, "Failed to duplicate element[0].");
            ret = -1;
            break;
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
        this->onFree(base);
        if (i >= j)
        {
            ret = this->_sort(objs, elemSize, size, type, begin, i - 1);
            if (ret)
            {
                break;
            }
            ret = this->_sort(objs, elemSize, size, type, i + 1, end);
        }
    } while (0);
    return ret;
}

int SortQuickObject::onSort(void * objs, int elemSize, int size, SortType type)
{
    return _sort(objs, elemSize, size, type, 0, size - 1);
}

