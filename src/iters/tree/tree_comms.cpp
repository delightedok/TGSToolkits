#include "../../comms/comm_headers.h"
#include <iters/tree/tree_comms.h>

#define THIS_FILE "tree_comms.cpp"
#define LOG_TAG "ITERS-TREE-COMMS"

struct TreeNodeDesc
{
    TreeNode * node;
    unsigned int nchildren;
};

static void * iters_tree_on_duplicate_default(void * obj)
{
    return obj;
}

static void iters_tree_on_free_default(void * obj)
{
    COMM_UNUSED(obj);
}

static int iters_tree_on_compare_default(void * obj1, void * obj2)
{
    return obj1 == obj2 ? 0 : obj1 > obj2 ? 1 : -1;
}

static void * iters_tree_on_duplicate_node(void * obj)
{
    void * ret = NULL;
    TreeNode * src = NULL;
    do {
        src = (TreeNode *)obj;
        if (!src)
        {
            break;
        }
        ret = mmalloc(sizeof(TreeNode));
        if (!ret)
        {
            mlog_e(LOG_TAG, THIS_FILE, "Failed to alloc space for TreeNode!");
            break;
        }
        memcpy(ret, obj, sizeof(TreeNode));
    } while (0);
    return ret;
}

static void iters_tree_on_free_node(void * obj)
{
    mfree(obj);
}

TreeObject::TreeObject(TreeVTable & vtable, unsigned int nchildren)
{
    this->mDatas = NULL;
    this->mSize = 0;
    memcpy(&this->mVTable, &vtable, sizeof(TreeVTable));
    COMM_ASSIGN_IF(this->mVTable.onDuplicate, iters_tree_on_duplicate_default, !mVTable.onDuplicate);
    COMM_ASSIGN_IF(this->mVTable.onFree, iters_tree_on_free_default, !mVTable.onFree);
    COMM_ASSIGN_IF(this->mVTable.onCompare, iters_tree_on_compare_default, !mVTable.onCompare);
}

TGSTK_EXPORT int TreeObject::size(void)
{
    return this->mSize;
}

TGSTK_EXPORT int TreeObject::iterate(Func_itersTree3 onIterate, void * arg,
    TreeIterateMethod method)
{
    COMM_ASSERT_RETURN(onIterate, -1);

    int ret = -1;
    switch (method)
    {
        case emIterTreeIterMethodBFs:
            {
                ListVTable vtable = { iters_tree_on_duplicate_node, iters_tree_on_free_node };
                NormalListObject list(vtable);
                list.rpush(this->mDatas, sizeof(TreeNode *));
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

void * TreeObject::doDuplicate(void * obj)
{
    return this->mVTable.onDuplicate(obj);
}

void TreeObject::doFree(void * obj)
{
    this->mVTable.onFree(obj);
}

int TreeObject::doCompare(void * obj1, void * obj2)
{
    return this->mVTable.onCompare(obj1, obj2);
}

int TreeObject::iterate_bfs(NormalListObject & list, Func_itersTree3 onIterate, void * arg)
{
    int ret = 0;
    TreeNode * node = NULL;
    TreeNode * p = NULL;

    do {
        /* Find a NOT NULL Node */
        do {
            node = (TreeNode *)list.lpop();
        } while (list.size() > 0 && !node);

        if (!node)  // All Nodes in tree has been iterated
        {
            break;
        }

        p = node->child;
        while (NULL != p)
        {
            list.rpush(p, sizeof(TreeNode *));
            p = p->sibling;
        }

        ret = onIterate(node->ptr.ptr, arg);

        iters_tree_on_free_node(node);

        if (ret)  // Failed to handle the Data in Node
        {
            break;
        }

        ret = this->iterate_bfs(list, onIterate, arg);
    } while (0);
    
    return ret;
}

int TreeObject::iterate_dfs(TreeNode * root, Func_itersTree3 onIterate, void * arg)
{
    COMM_ASSERT_RETURN(root, 0);

    int ret = 0;
    TreeNode * p = NULL;
    do {
        ret = onIterate(root->ptr.ptr, arg);
        if (ret)  // Failed to handle the Data in Node
        {
            break;
        }

        p = root->child;
        while (NULL != p && !ret)
        {
            ret = this->iterate_dfs(p, onIterate, arg);
            p = p->sibling;
        }
    } while (0);
    return ret;
}

