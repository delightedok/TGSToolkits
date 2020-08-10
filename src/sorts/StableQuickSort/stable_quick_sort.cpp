#include "../../comms/comm_headers.h"
#include <iters/list/normal_list.h>
#include <sorts/stable_quick_sort.h>

#define THIS_FILE "stable_quick_sort.cpp"
#define LOG_TAG "SORTS-STABLE-QUICK"

struct StableQuickSortElementsIterator
{
    void * objs;
    int elemSize;
    int index;
    SortStableQuickObject * self;
};

TGSTK_EXPORT SortStableQuickObject::SortStableQuickObject(SortVTable & vTable) : SortObject(vTable)
{}

int SortStableQuickObject::on_sort_list_iterate(void * element, void * arg)
{
    COMM_ASSERT_RETURN(element && arg, -1);

    StableQuickSortElementsIterator * itor = (StableQuickSortElementsIterator *)arg;
    COMM_ASSERT_RETURN(itor->objs && itor->self, -1);

    itor->self->onCopy(COMM_ARRAY_ELEM(itor->objs, itor->elemSize, itor->index), element);
    itor->index++;

    return 0;
}

int SortStableQuickObject::_sort(void * objs, int elemSize, int size, SortType type,
    int begin, int end, void * tempObj)
{
    COMM_ASSERT_RETURN(objs && size > 0 && size >= end && begin >= 0, -1);

    int ret = 0;
    int i = 0;
    int rc = 0;
    ListVTable vtable = { this->get_vtable().onDuplicate, this->get_vtable().onFree };
    NormalListObject leftList(vtable), rightList(vtable);
    void * base = tempObj;

    do {
        if (begin >= end)  // only one element should be handled
        {
            break;
        }

        if (!tempObj)  // NOT alloc memory outside the function, then alloc it in the function
        {
            base = this->onDuplicate(COMM_ARRAY_ELEM(objs, elemSize, begin));
            if (!base)
            {
                mlog_e(LOG_TAG, THIS_FILE, "Failed to duplicate element[0].");
                ret = -1;
                break;
            }
        } else
        {
            this->onCopy(base, COMM_ARRAY_ELEM(objs, elemSize, begin));
        }

        for (i = begin + 1; i <= end && !ret; i++)
        {
            rc = this->onCompare(base, COMM_ARRAY_ELEM(objs, elemSize, i));
            switch (type)
            {
                case emSortDesc:
                    {
                        if (rc <= 0)
                        {
                            leftList.rpush(COMM_ARRAY_ELEM(objs, elemSize, i), elemSize);
                        } else
                        {
                            rightList.rpush(COMM_ARRAY_ELEM(objs, elemSize, i), elemSize);
                        }
                    }
                    break;
                case emSortAsc:
                    {
                        if (rc >= 0)
                        {
                            leftList.rpush(COMM_ARRAY_ELEM(objs, elemSize, i), elemSize);
                        } else
                        {
                            rightList.rpush(COMM_ARRAY_ELEM(objs, elemSize, i), elemSize);
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

        if (ret)
        {
            leftList.clear();
            rightList.clear();
            break;
        }

        int pivotPos = leftList.size() + begin;
        StableQuickSortElementsIterator iterators = { objs, elemSize, begin, this };

        leftList.iterate(on_sort_list_iterate, &iterators);

        this->onCopy(COMM_ARRAY_ELEM(objs, elemSize, pivotPos), base);

        iterators.objs = objs;
        iterators.elemSize = elemSize;
        iterators.index = pivotPos + 1;
        iterators.self = this;
        rightList.iterate(on_sort_list_iterate, &iterators);

        leftList.clear();
        rightList.clear();

        ret = this->_sort(objs, elemSize, size, type, begin, pivotPos - 1, base);
        if (ret)
        {
            break;
        }
        ret = this->_sort(objs, elemSize, size, type, pivotPos + 1, end, base);

        if (!tempObj)  // Alloc memory in this function, then the free operation should be processed
        {
            this->onFree(base);
        }
    } while (0);
    return ret;
}

int SortStableQuickObject::onSort(void * objs, int elemSize, int size, SortType type)
{
    return _sort(objs, elemSize, size, type, 0, size - 1, NULL);
}

