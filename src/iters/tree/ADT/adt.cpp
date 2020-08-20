#include "../../../comms/comm_headers.h"
#include <iters/tree/adt.h>

#define THIS_FILE "adt.cpp"
#define LOG_TAG "ITERS-TREE-ADT"

TGSTK_EXPORT ADTObject::ADTObject(TreeVTable & vtable) : TreeObject(vtable, 2)
{
    this->mDatas = NULL;
}

ADTNode * ADTObject::make_node(void)
{
    ADTNode * ret = NULL;
    do {
        ret = (ADTNode *)mmalloc(sizeof(ADTNode));
        if (!ret)
        {
            mlog_e(LOG_TAG, THIS_FILE, "Failed to alloc space for ADTNode.");
            break;
        }
        memset(ret, 0, sizeof(ADTNode));
    } while (0);
    return ret;
}

void ADTObject::del_node(ADTNode * node)
{
    mfree(node);
}

TGSTK_EXPORT int ADTObject::push(void  * data, unsigned int size)
{
    return this->push(&this->mDatas, data, size);
}

int ADTObject::push(ADTNode ** root, void  * data, unsigned int size)
{
    COMM_ASSERT_RETURN(root, -1);

    int ret = -1;
    int rc = 0;
    ADTNode * _root = *root;
    do {
        if (!_root)
        {
            ADTNode * node = this->make_node();
            if (!node)
            {
                mlog_e(LOG_TAG, THIS_FILE, "Failed to create a ADT Node.");
                break;
            }
            node->ptr.ptr = this->doDuplicate(data);
            node->ptr.size = size;
            *root = node;
            ret = 0;
            break;
        }
        rc = this->doCompare(_root->ptr.ptr, data);
        if (rc <= 0)
        {
            ret = this->push(&_root->childs[0], data, size);
        } else
        {
            ret = this->push(&_root->childs[1], data, size);
        }
    } while (0);
    return ret;
}

TGSTK_EXPORT void * ADTObject::pop(int pos)
{
    return NULL;
}

TGSTK_EXPORT void * ADTObject::pop(void * obj)
{
    return NULL;
}

static void * iters_adt_on_duplicate_node(void * obj)
{
    void * ret = NULL;
    ADTNode * src = NULL;
    do {
        src = (ADTNode *)obj;
        if (!src)
        {
            break;
        }
        ret = mmalloc(sizeof(ADTNode));
        if (!ret)
        {
            mlog_e(LOG_TAG, THIS_FILE, "Failed to alloc space for ADTNode!");
            break;
        }
        memcpy(ret, obj, sizeof(ADTNode));
    } while (0);
    return ret;
}

static void iters_adt_on_free_node(void * obj)
{
    mfree(obj);
}

TGSTK_EXPORT int ADTObject::iterate(Func_itersTree3 onIterate, void * arg,
    TreeIterateMethod method)
{
    COMM_ASSERT_RETURN(onIterate, -1);

    int ret = -1;
    switch (method)
    {
        case emIterTreeIterMethodBFs:
            {
                ListVTable vtable = { iters_adt_on_duplicate_node, iters_adt_on_free_node };
                NormalListObject list(vtable);
                list.rpush(this->mDatas, sizeof(ADTNode *));
                ret = this->iterate_bfs(list, onIterate, arg);
            }
            break;
        case emIterTreeIterMethodDFs:
            {
                ret = this->iterate_dfs(this->mDatas, onIterate, arg);
            }
            break;
        default:
            {}
            break;
    }
    return ret;
}

int ADTObject::iterate_bfs(NormalListObject & list, Func_itersTree3 onIterate, void * arg)
{
    int ret = 0;
    ADTNode * node = NULL;

    do {
        /* Find a NOT NULL Node */
        do {
            node = (ADTNode *)list.lpop();
        } while (list.size() > 0 && !node);

        if (!node)  // All Nodes in tree has been iterated
        {
            break;
        }

        list.rpush(node->childs[0], sizeof(ADTNode *));
        list.rpush(node->childs[1], sizeof(ADTNode *));

        ret = onIterate(node->ptr.ptr, arg);

        iters_adt_on_free_node(node);

        if (ret)  // Failed to handle the Data in Node
        {
            break;
        }

        ret = this->iterate_bfs(list, onIterate, arg);
    } while (0);
    
    return ret;
}

int ADTObject::iterate_dfs(ADTNode * root, Func_itersTree3 onIterate, void * arg)
{
    COMM_ASSERT_RETURN(root, 0);

    int ret = 0;
    unsigned int i = 0;
    do {
        ret = onIterate(root->ptr.ptr, arg);
        if (ret)  // Failed to handle the Data in Node
        {
            break;
        }

        for (i = 0; i < 2 && !ret; i++)
        {
            ret = this->iterate_dfs(root->childs[i], onIterate, arg);
        }
    } while (0);
    return ret;
}

