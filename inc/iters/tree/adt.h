#ifndef __ITERS_TREE_ADT_H_
#define __ITERS_TREE_ADT_H_


#include <iters/tree/tree_comms.h>


struct ADTNode
{
    PointerT ptr;
    ADTNode * childs[2];
};


class ADTObject : public TreeObject
{
public:
    TGSTK_EXPORT ADTObject(TreeVTable & vtable);
    TGSTK_EXPORT virtual int push(void  * data, unsigned int size);
    TGSTK_EXPORT virtual void * pop(int pos = 0);
    TGSTK_EXPORT virtual void * pop(void * obj);
    TGSTK_EXPORT virtual int iterate(Func_itersTree3 onIterate, void * arg,
        TreeIterateMethod method = emIterTreeIterMethodBFs);
private:
    int push(ADTNode ** root, void  * data, unsigned int size);
    ADTNode * make_node(void);
    void del_node(ADTNode * node);
protected:
    virtual int iterate_bfs(NormalListObject & list, Func_itersTree3 onIterate, void * arg);
    virtual int iterate_dfs(ADTNode * root, Func_itersTree3 onIterate, void * arg);
private:
    ADTNode * mDatas;
};


#endif