#include "../comms/tester_comms.h"
#include <sorts/optimized_gnome_sort.h>
#include <string>

using namespace std;

#define THIS_FILE "tester_optimized_gnome_sorts.cpp"
#define LOG_TAG "TESTER-OPTIMIZED-GNOME-SORTS"

static int tester_optimized_gnome_sorts_on_compare(void * obj1, void * obj2)
{
    int * p1 = (int *)obj1;
    int * p2 = (int *)obj2;
    return *p1 > *p2 ? 1 : (*p1 == *p2 ? 0 : -1);
}

static int tester_optimized_gnome_sorts_on_exchange(void * obj1, void * obj2)
{
    TESTER_ASSERT_RETURN(obj1 && obj2, -1);

    int * p1 = (int *)obj1;
    int * p2 = (int *)obj2;
    int tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
    return 0;
}

static void * tester_optimized_gnome_sorts_on_duplicate(void * obj)
{
    int * ret = new int;
    *ret = *(int *)obj;
    return ret;
}

static void tester_optimized_gnome_sorts_on_free(void * obj)
{
    delete (int *)obj;
}

static void tester_optimized_gnome_sorts_on_copy(void * dst, void * src)
{
    *(int *)dst = *(int *)src;
}

int main(int argc, char * args[])
{
    unsigned int i = 0;
    int a[10] = {8, 7, 4, 9, 6, 2, 5, 7, 0, 8};
    int b[10] = {8, 7, 4, 9, 6, 2, 5, 7, 0, 8};
    int c[10] = {8, 7, 4, 9, 6, 2, 5, 7, 0, 8};
    int d[10] = {8, 7, 4, 9, 6, 2, 5, 7, 0, 8};

    SortVTable vTable;
    vTable.onCompare = tester_optimized_gnome_sorts_on_compare;
    vTable.onExchange = tester_optimized_gnome_sorts_on_exchange;
    vTable.onDuplicate = tester_optimized_gnome_sorts_on_duplicate;
    vTable.onFree = tester_optimized_gnome_sorts_on_free;
    vTable.onCopy = tester_optimized_gnome_sorts_on_copy;
    SortOptimizedGnomeObject obj(vTable);

    for (i = 0; i < sizeof(a) / sizeof(a[0]); i++)
    {
        tester_logd(LOG_TAG, THIS_FILE, "a[%d]: %d", i, a[i]);
    }
    tester_logd(LOG_TAG, THIS_FILE, "-------------------------------------------------");
    for (i = 0; i < sizeof(b) / sizeof(b[0]); i++)
    {
        tester_logd(LOG_TAG, THIS_FILE, "b[%d]: %d", i, b[i]);
    }
    tester_logd(LOG_TAG, THIS_FILE, "-------------------------------------------------");

    obj.sort(a, sizeof(a[0]), sizeof(a) / sizeof(a[0]), emSortAsc);
    for (i = 0; i < sizeof(a) / sizeof(a[0]); i++)
    {
        tester_logd(LOG_TAG, THIS_FILE, "a[%d]: %d", i, a[i]);
    }
    tester_logd(LOG_TAG, THIS_FILE, "Statistic: %s", obj.get_statistic().c_str());
    tester_logd(LOG_TAG, THIS_FILE, "-------------------------------------------------");

    obj.sort(b, sizeof(b[0]), sizeof(b) / sizeof(b[0]), emSortDesc);
    for (i = 0; i < sizeof(b) / sizeof(b[0]); i++)
    {
        tester_logd(LOG_TAG, THIS_FILE, "b[%d]: %d", i, b[i]);
    }
    tester_logd(LOG_TAG, THIS_FILE, "Statistic: %s", obj.get_statistic().c_str());
    tester_logd(LOG_TAG, THIS_FILE, "-------------------------------------------------");

    obj.sort2(c, sizeof(c[0]), sizeof(c) / sizeof(c[0]), emSortAsc);
    for (i = 0; i < sizeof(c) / sizeof(c[0]); i++)
    {
        tester_logd(LOG_TAG, THIS_FILE, "c[%d]: %d", i, c[i]);
    }
    tester_logd(LOG_TAG, THIS_FILE, "Statistic: %s", obj.get_statistic().c_str());
    tester_logd(LOG_TAG, THIS_FILE, "-------------------------------------------------");

    obj.sort2(d, sizeof(d[0]), sizeof(d) / sizeof(d[0]), emSortDesc);
    for (i = 0; i < sizeof(d) / sizeof(d[0]); i++)
    {
        tester_logd(LOG_TAG, THIS_FILE, "d[%d]: %d", i, d[i]);
    }
    tester_logd(LOG_TAG, THIS_FILE, "Statistic: %s", obj.get_statistic().c_str());
    tester_logd(LOG_TAG, THIS_FILE, "-------------------------------------------------");
    return 0;
}

