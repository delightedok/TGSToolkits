#ifndef __TREE_COMMS_H_
#define __TREE_COMMS_H_


#include <tgstk_comms.h>
#include <comms/comms_types.h>
#include <iters/list/normal_list.h>


typedef void * (* Func_itersTree1)(void * obj);
typedef void (* Func_itersTree2)(void * obj);
typedef int (* Func_itersTree3)(void * obj, void * arg);
typedef int (* Func_itersTree4)(void * obj1, void * obj2);


struct TreeNode
{
    PointerT ptr;
    TreeNode * parent;
    TreeNode * sibling;
    TreeNode * child;
};


enum TreeIterateMethod
{
    emIterTreeIterMethodBFs,  // Breadth First
    emIterTreeIterMethodDFs,  // Depth First
};


struct TreeVTable
{
    Func_itersTree1 onDuplicate;
    Func_itersTree2 onFree;
    Func_itersTree4 onCompare;
};


class TreeObject
{
public:
    TreeObject(TreeVTable & vtable, unsigned int nchildren);
    virtual int push(void  * data, unsigned int size) = 0;
    virtual void * pop(int pos) = 0;
    virtual void * pop(void * obj) = 0;
    TGSTK_EXPORT virtual int size(void);
    TGSTK_EXPORT virtual int iterate(Func_itersTree3 onIterate, void * arg,
        TreeIterateMethod method = emIterTreeIterMethodBFs);
protected:
    void * doDuplicate(void * obj);
    void doFree(void * obj);
    int doCompare(void * obj1, void * obj2);
    virtual int iterate_bfs(NormalListObject & list, Func_itersTree3 onIterate, void * arg);
    virtual int iterate_dfs(TreeNode * root, Func_itersTree3 onIterate, void * arg);
private:
    TreeVTable mVTable;
protected:
    TreeNode * mDatas;
    unsigned int mSize;
};


#endif