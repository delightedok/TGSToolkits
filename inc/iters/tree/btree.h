#ifndef __ITERS_TREE_BTREE_H_
#define __ITERS_TREE_BTREE_H_


#include <iters/tree/tree_comms.h>


class BTreeObject : public TreeObject
{
public:
    TGSTK_EXPORT BTreeObject(TreeVTable & vtable);
    TGSTK_EXPORT virtual int push(void  * data, unsigned int size);
    TGSTK_EXPORT virtual void * pop(int pos = 0);
    TGSTK_EXPORT virtual void * pop(void * obj);
private:
    int push(TreeNode ** root, void  * data, unsigned int size);
protected:
private:
    TreeNode * mLastNode;
};


#endif