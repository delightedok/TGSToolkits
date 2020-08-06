#include "../../comms/comm_headers.h"
#include <sorts/optimized_gnome_sort.h>

#define THIS_FILE "optimized_gnome_sort.cpp"
#define LOG_TAG "SORTS-OPTIMIZED-GNOME"

TGSTK_EXPORT SortOptimizedGnomeObject::SortOptimizedGnomeObject(SortVTable & vTable) : SortObject(vTable)
{}

TGSTK_DEPRECATED int SortOptimizedGnomeObject::onSort1(void * objs, int elemSize, int size, SortType type)
{
    COMM_ASSERT_RETURN(objs && size > 0, -1);

    int ret = 0;
    int i = 0;
    int rc = 0;
    int lastPos = -1;

    for (i = 1; i < size && !ret;)
    {
        rc = this->onCompare(COMM_ARRAY_ELEM(objs, elemSize, i - 1), COMM_ARRAY_ELEM(objs, elemSize, i));
        switch (type)
        {
            case emSortDesc:
                {
                    if (rc >= 0)
                    {
                        if (lastPos < 0)
                        {
                            i++;
                        } else
                        {
                            i = lastPos + 1;  // jump to the next index of the last max index that compared
                            lastPos = -1;
                        }
                    } else
                    {
                        ret = this->onExchange(COMM_ARRAY_ELEM(objs, elemSize, i - 1),
                            COMM_ARRAY_ELEM(objs, elemSize, i));
                        if (lastPos < 0)  // record the last max index that compared
                        {
                            lastPos = i;
                        }
                        i--;
                        if (i <= 0)  // can NOT compare back anymore
                        {
                            i = lastPos + 1;  // jump to the next index of the last max index that compared
                            lastPos = -1;
                        }
                    }
                }
                break;
            case emSortAsc:
                {
                    if (rc <= 0)
                    {
                        if (lastPos < 0)
                        {
                            i++;
                        } else
                        {
                            i = lastPos + 1;  // jump to the next index of the last max index that compared
                            lastPos = -1;
                        }
                    } else
                    {
                        ret = this->onExchange(COMM_ARRAY_ELEM(objs, elemSize, i - 1),
                            COMM_ARRAY_ELEM(objs, elemSize, i));
                        if (lastPos < 0)  // record the last max index that compared
                        {
                            lastPos = i;
                        }
                        i--;
                        if (i <= 0)  // can NOT compare back anymore
                        {
                            i = lastPos + 1;  // jump to the next index of the last max index that compared
                            lastPos = -1;
                        }
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

int SortOptimizedGnomeObject::onSort(void * objs, int elemSize, int size, SortType type)
{
    COMM_ASSERT_RETURN(objs && size > 0, -1);

    int ret = 0;
    int i = 0;
    int rc = 0;
    int beenBack = 0;
    int lastPos = -1;
    void * _obj = NULL;

    do {
        if (1 == size)
        {
            break;
        }
        _obj = this->onDuplicate(COMM_ARRAY_ELEM(objs, elemSize, 1));
        if (!_obj)
        {
            mlog_e(LOG_TAG, THIS_FILE, "Failed to duplicate element[1].");
            ret = -1;
            break;
        }
        lastPos = 1;

        for (i = 0; i < size - 1 && !ret;)
        {
            rc = this->onCompare(COMM_ARRAY_ELEM(objs, elemSize, i), _obj);
            switch (type)
            {
                case emSortDesc:
                    {
                        if (rc >= 0)
                        {
                            if (beenBack)  // has gone backward
                            {
                                this->onCopy(COMM_ARRAY_ELEM(objs, elemSize, i + 1), _obj);
                                beenBack = 0;
                            }
                            i = lastPos++;  // jump to the next index of the last max index that compared
                            this->onCopy(_obj, COMM_ARRAY_ELEM(objs, elemSize, lastPos));
                        } else
                        {
                            this->onCopy(COMM_ARRAY_ELEM(objs, elemSize, i + 1),
                                COMM_ARRAY_ELEM(objs, elemSize, i));
                            i--;
                            beenBack = 1;
                            if (i < 0)  // can NOT compare back anymore
                            {
                                this->onCopy(COMM_ARRAY_ELEM(objs, elemSize, 0), _obj);
                                i = lastPos++;  // jump to the next index of the last max index that compared
                                this->onCopy(_obj, COMM_ARRAY_ELEM(objs, elemSize, lastPos));
                            }
                        }
                    }
                    break;
                case emSortAsc:
                    {
                        if (rc <= 0)
                        {
                            if (beenBack)  // has gone backward
                            {
                                this->onCopy(COMM_ARRAY_ELEM(objs, elemSize, i + 1), _obj);
                                beenBack = 0;
                            }
                            i = lastPos++;  // jump to the next index of the last max index that compared
                            this->onCopy(_obj, COMM_ARRAY_ELEM(objs, elemSize, lastPos));
                        } else
                        {
                            this->onCopy(COMM_ARRAY_ELEM(objs, elemSize, i + 1),
                                COMM_ARRAY_ELEM(objs, elemSize, i));
                            i--;
                            beenBack = 1;
                            if (i < 0)  // can NOT compare back anymore
                            {
                                this->onCopy(COMM_ARRAY_ELEM(objs, elemSize, 0), _obj);
                                i = lastPos++;  // jump to the next index of the last max index that compared
                                this->onCopy(_obj, COMM_ARRAY_ELEM(objs, elemSize, lastPos));
                            }
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
    } while (0);
    return ret;
}

