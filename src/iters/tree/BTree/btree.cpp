#include "../../../comms/comm_headers.h"
#include <iters/tree/btree.h>

#define THIS_FILE "btree.cpp"
#define LOG_TAG "ITERS-TREE-BTREE"

TGSTK_EXPORT BTreeObject::BTreeObject(TreeVTable & vtable) : TreeObject(vtable, 2)
{
    this->mLastNode = NULL;
}

TGSTK_EXPORT int BTreeObject::push(void  * data, unsigned int size)
{
    return this->push(&this->mDatas, data, size);
}

int BTreeObject::push(TreeNode ** root, void  * data, unsigned int size)
{
    COMM_ASSERT_RETURN(root, -1);

    int ret = -1;
    int rc = 0;
    TreeNode * _root = *root;
    do {
        if (!*root)
        {
            TreeNode * node = this->new_node();
            if (!node)
            {
                mlog_e(LOG_TAG, THIS_FILE, "Failed to create a Tree Node.");
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

