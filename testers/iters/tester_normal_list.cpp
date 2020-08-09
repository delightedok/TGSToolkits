#include "../comms/tester_comms.h"
#include <iters/list/normal_list.h>
#include <string>

using namespace std;

#define THIS_FILE "tester_normal_list.cpp"
#define LOG_TAG "TESTER-NORMAL-LIST"

static void * tester_normal_list_on_duplicate(void * obj)
{
    return obj;
}

static void tester_normal_list_on_free(void * obj)
{
    (void)obj;
}

static int tester_normal_list_on_iterate_1(void * element, void * arg)
{
    /* note: long here to make compiler happy in x64 */
    tester_logd(LOG_TAG, THIS_FILE, "element: %ld", (long)element);
    return 0;
}

int main(int argc, char * args[])
{
    ListVTable vtable;
    vtable.onDuplicate = tester_normal_list_on_duplicate;
    vtable.onFree = tester_normal_list_on_free;
    NormalListObject obj1(vtable);
    NormalListObject obj2(vtable);
    for (int i = 0; i < 10; i++)
    {
        obj1.rpush((void *)(long)i, sizeof(int));
    }
    obj1.iterate(tester_normal_list_on_iterate_1, NULL, 0);
    tester_logd(LOG_TAG, THIS_FILE, "-------------------------------------------------");
    for (int i = 0; i < 10; i++)
    {
        obj2.lpush((void *)(long)i, sizeof(int));
    }
    obj2.iterate(tester_normal_list_on_iterate_1, NULL, 1);
    tester_logd(LOG_TAG, THIS_FILE, "-------------------------------------------------");
    for (int i = 0; i < 5; i++)
    {
        long a = (long)obj1.rpop();
        tester_logd(LOG_TAG, THIS_FILE, "pop %ld", a);
    }
    obj1.iterate(tester_normal_list_on_iterate_1, NULL, 0);
    tester_logd(LOG_TAG, THIS_FILE, "-------------------------------------------------");
    for (int i = 0; i < 5; i++)
    {
        long a = (long)obj2.lpop();
        tester_logd(LOG_TAG, THIS_FILE, "pop %ld", a);
    }
    obj2.iterate(tester_normal_list_on_iterate_1, NULL, 0);
    tester_logd(LOG_TAG, THIS_FILE, "-------------------------------------------------");
    return 0;
}

