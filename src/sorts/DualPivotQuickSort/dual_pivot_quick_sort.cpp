#include "../../comms/comm_headers.h"
#include <sorts/dual_pivot_quick_sort.h>

#define THIS_FILE "dual_pivot_quick_sort.cpp"
#define LOG_TAG "SORTS-DUAL-PIVOT-QUICK"

TGSTK_EXPORT SortDualPivotQuickObject::SortDualPivotQuickObject(SortVTable & vTable) : SortObject(vTable)
{}
#include <stdio.h>
void display(void * array, int size, const char * prefix)
{
    int i = 0;
    int * aa = (int *)array;
    printf("%s", prefix);
    for (i = 0; i < size; i++)
    {
        printf("%d ", aa[i]);
    }
    printf("\n");
}
int SortDualPivotQuickObject::_sort(void * objs, int elemSize, int size, SortType type,
    int begin, int end, void * tempObj1, void * tempObj2)
{
    COMM_ASSERT_RETURN(objs && size > 0 && size >= end && begin >= 0, -1);

    int ret = 0;
    int i = begin, j = end, k = begin;
    int rc = 0;
    void * left = tempObj1;
    void * right = tempObj2;

    do {
        if (begin >= end)  // only one element should be handled
        {
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

        rc = this->onCompare(COMM_ARRAY_ELEM(objs, elemSize, i), COMM_ARRAY_ELEM(objs, elemSize, j));
        if ((emSortDesc == type && rc < 0) || (emSortAsc == type && rc > 0))
        {
            this->onExchange(COMM_ARRAY_ELEM(objs, elemSize, i), COMM_ARRAY_ELEM(objs, elemSize, j));
        }

        if (!tempObj1)  // NOT alloc memory outside the function, then alloc it in the function
        {
            left = this->onDuplicate(COMM_ARRAY_ELEM(objs, elemSize, i));
            if (!left)
            {
                mlog_e(LOG_TAG, THIS_FILE, "Failed to duplicate element[%d].", i);
                ret = -1;
                break;
            }
        } else
        {
            this->onCopy(left, COMM_ARRAY_ELEM(objs, elemSize, i));
        }
        if (!tempObj2)  // NOT alloc memory outside the function, then alloc it in the function
        {
            right = this->onDuplicate(COMM_ARRAY_ELEM(objs, elemSize, j));
            if (!right)
            {
                mlog_e(LOG_TAG, THIS_FILE, "Failed to duplicate element[%d].", j);
                ret = -1;
                break;
            }
        } else
        {
            this->onCopy(right, COMM_ARRAY_ELEM(objs, elemSize, j));
        }

        i = begin + 1;
        j = end - 1;
        k = i;

        while (k <= j)
        {
            rc = this->onCompare(COMM_ARRAY_ELEM(objs, elemSize, k), left);
            if ((emSortDesc == type && rc > 0) || (emSortAsc == type && rc < 0))
            {
                this->onExchange(COMM_ARRAY_ELEM(objs, elemSize, k), COMM_ARRAY_ELEM(objs, elemSize, i));
                i++;
            } else
            {
                rc = this->onCompare(COMM_ARRAY_ELEM(objs, elemSize, k), right);
                if ((emSortDesc == type && rc <= 0) || (emSortAsc == type && rc >= 0))
                {
                    rc = this->onCompare(COMM_ARRAY_ELEM(objs, elemSize, j), right);
                    while (((emSortDesc == type && rc < 0) || (emSortAsc == type && rc > 0)) && k < j)
                    {
                        j--;
                        rc = this->onCompare(COMM_ARRAY_ELEM(objs, elemSize, j), right);
                    }
                    this->onExchange(COMM_ARRAY_ELEM(objs, elemSize, k), COMM_ARRAY_ELEM(objs, elemSize, j));
                    j--;

                    rc = this->onCompare(COMM_ARRAY_ELEM(objs, elemSize, k), left);
                    if ((emSortDesc == type && rc > 0) || (emSortAsc == type && rc < 0))
                    {
                        this->onExchange(COMM_ARRAY_ELEM(objs, elemSize, k), COMM_ARRAY_ELEM(objs, elemSize, i));
                        i++;
                    }
                }
            }
            k++;
        }
        i--;
        j++;
        this->onExchange(COMM_ARRAY_ELEM(objs, elemSize, begin), COMM_ARRAY_ELEM(objs, elemSize, i));
        this->onExchange(COMM_ARRAY_ELEM(objs, elemSize, end), COMM_ARRAY_ELEM(objs, elemSize, j));

        ret = _sort(objs, elemSize, size, type, begin, i - 1, left, right);
        if (ret)
        {
            break;
        }
        ret = _sort(objs, elemSize, size, type, i + 1, j - 1, left, right);
        if (ret)
        {
            break;
        }
        ret = _sort(objs, elemSize, size, type, j + 1, end, left, right);
        if (ret)
        {
            break;
        }

    } while (0);

    if (!tempObj1)  // Alloc memory in this function, then the free operation should be processed
    {
        this->onFree(left);
    }
    if (!tempObj2)  // Alloc memory in this function, then the free operation should be processed
    {
        this->onFree(right);
    }

    return ret;
}

int SortDualPivotQuickObject::onSort(void * objs, int elemSize, int size, SortType type)
{
    return _sort(objs, elemSize, size, type, 0, size - 1, NULL, NULL);
}

