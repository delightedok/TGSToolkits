#ifndef __ITERS_TREE_AVL_H_
#define __ITERS_TREE_AVL_H_


#include <iters/tree/tree_comms.h>


struct AVLNode
{
    PointerT ptr;
    int height;
    AVLNode * childs[2];
};


class AVLObject : public TreeObject
{
public:
    TGSTK_EXPORT AVLObject(TreeVTable & vtable);
    TGSTK_EXPORT virtual int push(void  * data, unsigned int size);
    TGSTK_EXPORT virtual void pop(void * obj);
    TGSTK_EXPORT virtual int height(void);
    TGSTK_EXPORT virtual int iterate(Func_itersTree3 onIterate, void * arg,
        TreeIterateMethod method = emIterTreeIterMethodBFs);
private:
    int push(AVLNode ** root, void  * data, unsigned int size);
    AVLNode * pop(AVLNode ** root, void * obj);
    AVLNode * find_max_node(AVLNode * root);
    AVLNode * find_min_node(AVLNode * root);
    AVLNode * make_node(void);
    void del_node(AVLNode * node);
    int height(AVLNode * root);
    AVLNode * rotate_L(AVLNode * node);  // rotate Left
    AVLNode * rotate_R(AVLNode * node);  // rotate Right
    AVLNode * rotate_LR(AVLNode * node);  // rotate Left and Right
    AVLNode * rotate_RL(AVLNode * node);  // rotate Right and Left
protected:
    virtual int iterate_bfs(NormalListObject & list, Func_itersTree3 onIterate, void * arg);
    virtual int iterate_dfs(AVLNode * root, Func_itersTree3 onIterate, void * arg);
private:
    AVLNode * mDatas;
};


#endif

