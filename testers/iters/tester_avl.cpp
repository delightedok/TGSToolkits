#include "../comms/tester_comms.h"
#include <iters/tree/avl.h>
#include <string>

using namespace std;

#define THIS_FILE "tester_avl.cpp"
#define LOG_TAG "TESTER-AVL"

static void * tester_avl_on_duplicate(void * obj)
{
    return obj;
}

static void tester_avl_on_free(void * obj)
{
    (void)obj;
}

static int tester_avl_on_compare(void * obj1, void * obj2)
{
    return obj1 == obj2 ? 0 : obj1 > obj2 ? 1 : -1;
}

static int tester_avl_on_iterate_1(void * element, void * arg)
{
    /* note: long here to make compiler happy in x64 */
    tester_logd(LOG_TAG, THIS_FILE, "element: %ld", (long)element);
    return 0;
}

int main(int argc, char * args[])
{
    TreeVTable vtable;
    vtable.onDuplicate = tester_avl_on_duplicate;
    vtable.onFree = tester_avl_on_free;
    vtable.onCompare = tester_avl_on_compare;
    AVLObject obj1(vtable);
    for (int i = 0; i < 10; i++)
    {
        obj1.push((void *)(long)i, sizeof(int));
    }
    obj1.iterate(tester_avl_on_iterate_1, NULL, emIterTreeIterMethodBFs);
    obj1.iterate(tester_avl_on_iterate_1, NULL, emIterTreeIterMethodDFs);
    tester_logd(LOG_TAG, THIS_FILE, "-------------------------------------------------");
    for (int i = 0; i < 10; i += 2)
    {
        tester_logd(LOG_TAG, THIS_FILE, "pop %d", i);
        obj1.pop((void *)(long)i);
    }
    obj1.iterate(tester_avl_on_iterate_1, NULL, emIterTreeIterMethodBFs);
    obj1.iterate(tester_avl_on_iterate_1, NULL, emIterTreeIterMethodDFs);
    tester_logd(LOG_TAG, THIS_FILE, "-------------------------------------------------");
    return 0;
}



