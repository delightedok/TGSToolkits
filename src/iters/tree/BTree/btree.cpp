#include "../../../comms/comm_headers.h"
#include <iters/tree/btree.h>

#define THIS_FILE "btree.cpp"
#define LOG_TAG "ITERS-TREE-BTREE"

TGSTK_EXPORT BTreeObject::BTreeObject(TreeVTable & vtable) : TreeObject(vtable, 2)
{
    this->mDatas = NULL;
}

BTreeNode * BTreeObject::make_node(void)
{
    BTreeNode * ret = NULL;
    do {
        ret = (BTreeNode *)mmalloc(sizeof(BTreeNode));
        if (!ret)
        {
            mlog_e(LOG_TAG, THIS_FILE, "Failed to alloc space for BTreeNode.");
            break;
        }
        memset(ret, 0, sizeof(BTreeNode));
    } while (0);
    return ret;
}

void BTreeObject::del_node(BTreeNode * node)
{
    mfree(node);
}

TGSTK_EXPORT int BTreeObject::push(void  * data, unsigned int size)
{
    return this->push(&this->mDatas, data, size);
}

int BTreeObject::push(BTreeNode ** root, void  * data, unsigned int size)
{
    COMM_ASSERT_RETURN(root, -1);

    int ret = -1;
    int rc = 0;
    BTreeNode * _root = *root;
    do {
        if (!_root)
        {
            BTreeNode * node = this->make_node();
            if (!node)
            {
                mlog_e(LOG_TAG, THIS_FILE, "Failed to create a BTree Node.");
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

TGSTK_EXPORT void * BTreeObject::pop(int pos)
{
    return NULL;
}

TGSTK_EXPORT void * BTreeObject::pop(void * obj)
{
    return NULL;
}

static void * iters_btree_on_duplicate_node(void * obj)
{
    void * ret = NULL;
    BTreeNode * src = NULL;
    do {
        src = (BTreeNode *)obj;
        if (!src)
        {
            break;
        }
        ret = mmalloc(sizeof(BTreeNode));
        if (!ret)
        {
            mlog_e(LOG_TAG, THIS_FILE, "Failed to alloc space for BTreeNode!");
            break;
        }
        memcpy(ret, obj, sizeof(BTreeNode));
    } while (0);
    return ret;
}

static void iters_btree_on_free_node(void * obj)
{
    mfree(obj);
}

TGSTK_EXPORT int BTreeObject::iterate(Func_itersTree3 onIterate, void * arg,
    TreeIterateMethod method)
{
    COMM_ASSERT_RETURN(onIterate, -1);

    int ret = -1;
    switch (method)
    {
        case emIterTreeIterMethodBFs:
            {
                ListVTable vtable = { iters_btree_on_duplicate_node, iters_btree_on_free_node };
                NormalListObject list(vtable);
                list.rpush(this->mDatas, sizeof(BTreeNode *));
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

int BTreeObject::iterate_bfs(NormalListObject & list, Func_itersTree3 onIterate, void * arg)
{
    int ret = 0;
    BTreeNode * node = NULL;

    do {
        /* Find a NOT NULL Node */
        do {
            node = (BTreeNode *)list.lpop();
        } while (list.size() > 0 && !node);

        if (!node)  // All Nodes in tree has been iterated
        {
            break;
        }

        list.rpush(node->childs[0], sizeof(BTreeNode *));
        list.rpush(node->childs[1], sizeof(BTreeNode *));

        ret = onIterate(node->ptr.ptr, arg);

        iters_btree_on_free_node(node);

        if (ret)  // Failed to handle the Data in Node
        {
            break;
        }

        ret = this->iterate_bfs(list, onIterate, arg);
    } while (0);
    
    return ret;
}

int BTreeObject::iterate_dfs(BTreeNode * root, Func_itersTree3 onIterate, void * arg)
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

