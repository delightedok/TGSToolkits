#ifndef __ITERS_TREE_BTREE_H_
#define __ITERS_TREE_BTREE_H_


#include <iters/tree/tree_comms.h>


struct BTreeNode
{
    PointerT ptr;
    BTreeNode * childs[2];
};


class BTreeObject : public TreeObject
{
public:
    TGSTK_EXPORT BTreeObject(TreeVTable & vtable);
    TGSTK_EXPORT virtual int push(void  * data, unsigned int size);
    TGSTK_EXPORT virtual void * pop(int pos = 0);
    TGSTK_EXPORT virtual void * pop(void * obj);
    TGSTK_EXPORT virtual int iterate(Func_itersTree3 onIterate, void * arg,
        TreeIterateMethod method = emIterTreeIterMethodBFs);
private:
    int push(BTreeNode ** root, void  * data, unsigned int size);
    BTreeNode * make_node(void);
    void del_node(BTreeNode * node);
protected:
    virtual int iterate_bfs(NormalListObject & list, Func_itersTree3 onIterate, void * arg);
    virtual int iterate_dfs(BTreeNode * root, Func_itersTree3 onIterate, void * arg);
private:
    BTreeNode * mDatas;
};


#endif