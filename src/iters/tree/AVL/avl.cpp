#include "../../../comms/comm_headers.h"
#include <iters/tree/avl.h>
#include <maths/maths.h>

#define THIS_FILE "avl.cpp"
#define LOG_TAG "ITERS-TREE-AVL"

TGSTK_EXPORT AVLObject::AVLObject(TreeVTable & vtable) : TreeObject(vtable, 2)
{
    this->mDatas = NULL;
}

AVLNode * AVLObject::make_node(void)
{
    AVLNode * ret = NULL;
    do {
        ret = (AVLNode *)mmalloc(sizeof(AVLNode));
        if (!ret)
        {
            mlog_e(LOG_TAG, THIS_FILE, "Failed to alloc space for AVLNode.");
            break;
        }
        memset(ret, 0, sizeof(AVLNode));
    } while (0);
    return ret;
}

void AVLObject::del_node(AVLNode * node)
{
    mfree(node);
}

TGSTK_EXPORT int AVLObject::push(void  * data, unsigned int size)
{
    return this->push(&this->mDatas, data, size);
}

int AVLObject::push(AVLNode ** root, void  * data, unsigned int size)
{
    COMM_ASSERT_RETURN(root, -1);

    int ret = -1;
    int rc = 0;
    AVLNode * _root = *root;
    do {
        if (!_root)
        {
            AVLNode * node = this->make_node();
            if (!node)
            {
                mlog_e(LOG_TAG, THIS_FILE, "Failed to create a AVL Node.");
                break;
            }
            node->ptr.ptr = this->doDuplicate(data);
            node->ptr.size = size;
            node->height = 0;
            *root = node;
            ret = 0;
            break;
        }
        rc = this->doCompare(_root->ptr.ptr, data);
        if (rc <= 0)
        {
            ret = this->push(&_root->childs[1], data, size);
            if (!ret)  // success, then check about the height
            {
                if (this->height(_root->childs[1]) - this->height(_root->childs[0]) >= 2)
                {
                    rc = this->doCompare(data, _root->childs[1]->ptr.ptr);
                    if (rc < 0)
                    {
                        mlog_d(LOG_TAG, THIS_FILE, "L");
                        *root = this->rotate_L(_root);
                    } else
                    {
                        mlog_d(LOG_TAG, THIS_FILE, "LR");
                        *root = this->rotate_LR(_root);
                    }
                }
            }
        } else
        {
            ret = this->push(&_root->childs[0], data, size);
            if (!ret)  // success, then check about the height
            {
                if (this->height(_root->childs[0]) - this->height(_root->childs[1]) >= 2)
                {
                    rc = this->doCompare(data, _root->childs[0]->ptr.ptr);
                    if (rc > 0)
                    {
                        mlog_d(LOG_TAG, THIS_FILE, "R");
                        *root = this->rotate_R(_root);
                    } else
                    {
                        mlog_d(LOG_TAG, THIS_FILE, "RL");
                        *root = this->rotate_RL(_root);
                    }
                }
            }
        }
    } while (0);

    if (!ret && _root)  // success, update the height of root-node
    {
        _root->height = toolkit_maths_max(2, this->height(_root->childs[0]), this->height(_root->childs[1])) + 1;
    }

    return ret;
}

TGSTK_EXPORT void AVLObject::pop(void * obj)
{
    this->mDatas = this->pop(&this->mDatas, obj);
}

AVLNode * AVLObject::pop(AVLNode ** root, void * obj)
{
    COMM_ASSERT_RETURN(root && *root, NULL);

    AVLNode * ret = *root;
    int rc = 0;
    AVLNode * p = NULL;
    AVLNode * _root = *root;
    if (NULL != _root)
    {
        rc = this->doCompare(obj, _root->ptr.ptr);
        if (0 == rc)  // Find the Node
        {
            AVLNode * minNode = NULL;
            ret = _root;

            if (_root->childs[0] && _root->childs[1])  // Has Left and Right Children
            {
                minNode = this->find_min_node(_root->childs[1]);
                COMM_ASSERT_RETURN(minNode, _root);  // In fact, minNode is always NOT NULL
                memcpy(&_root->ptr, &minNode->ptr, sizeof(minNode->ptr));
                _root->childs[1] = this->pop(&_root->childs[1], minNode->ptr.ptr);  // Pop the Min Node in Right Child
            } else  // Only have Left or Right Child
            {
                ret = _root->childs[0] ? _root->childs[0] : _root->childs[1];
                p = _root;
            }

            if (this->height(_root->childs[0]) - this->height(_root->childs[1]) >= 2)
            {
                *root = this->rotate_R(_root);
            } else if (this->height(_root->childs[1]) - this->height(_root->childs[0]) >= 2)
            {
                *root = this->rotate_L(_root);
            }
        } else if (rc > 0)  // Find the Node from the Right Child
        {
            _root->childs[1] = this->pop(&_root->childs[1], obj);
        } else if (rc < 0)  // Find the Node from the Left Child
        {
            _root->childs[0] = this->pop(&_root->childs[0], obj);
        }
    }

    /* Cleanning Up */
    if (p)
    {
        this->doFree(p->ptr.ptr);
        this->del_node(p);
    }

    return ret;
}

AVLNode * AVLObject::find_max_node(AVLNode * root)
{
    AVLNode * ret = root;
    do {
        if (NULL == root->childs[1])  // Does NOT have the Right Child, which means the root node is the max node
        {
            break;
        }
        ret = this->find_max_node(root->childs[1]);
    } while (0);
    return ret;
}

AVLNode * AVLObject::find_min_node(AVLNode * root)
{
    AVLNode * ret = root;
    do {
        if (NULL == root->childs[0])  // Does NOT have the Left Child, which means the root node is the min node
        {
            break;
        }
        ret = this->find_min_node(root->childs[1]);
    } while (0);
    return ret;
}

static void * iters_avl_on_duplicate_node(void * obj)
{
    return obj;
}

static void iters_avl_on_free_node(void * obj)
{
}

TGSTK_EXPORT int AVLObject::iterate(Func_itersTree3 onIterate, void * arg,
    TreeIterateMethod method)
{
    COMM_ASSERT_RETURN(onIterate, -1);

    int ret = -1;
    switch (method)
    {
        case emIterTreeIterMethodBFs:
            {
                ListVTable vtable = { iters_avl_on_duplicate_node, iters_avl_on_free_node };
                NormalListObject list(vtable);
                list.rpush(this->mDatas, sizeof(AVLNode *));
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

int AVLObject::iterate_bfs(NormalListObject & list, Func_itersTree3 onIterate, void * arg)
{
    int ret = 0;
    AVLNode * node = NULL;

    do {
        /* Find a NOT NULL Node */
        do {
            node = (AVLNode *)list.lpop();
        } while (list.size() > 0 && !node);

        if (!node)  // All Nodes in tree has been iterated
        {
            break;
        }

        list.rpush(node->childs[0], sizeof(AVLNode *));
        list.rpush(node->childs[1], sizeof(AVLNode *));

        ret = onIterate(node->ptr.ptr, arg);

        iters_avl_on_free_node(node);

        if (ret)  // Failed to handle the Data in Node
        {
            break;
        }

        ret = this->iterate_bfs(list, onIterate, arg);
    } while (0);
    
    return ret;
}

int AVLObject::iterate_dfs(AVLNode * root, Func_itersTree3 onIterate, void * arg)
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

TGSTK_EXPORT int AVLObject::height(void)
{
    return this->height(this->mDatas);
}

TGSTK_EXPORT int AVLObject::height(AVLNode * root)
{
    COMM_ASSERT_RETURN(root, -1);

    return root->height;
}

AVLNode * AVLObject::rotate_L(AVLNode * node)
{
    COMM_ASSERT_RETURN(node, NULL);

    COMM_ASSERT_RETURN(node->childs[1], node);  // Has Right Child

    AVLNode * ret = node->childs[1];
    node->childs[1] = ret->childs[0];
    ret->childs[0] = node;

    node->height = toolkit_maths_max(2, this->height(node->childs[0]), this->height(node->childs[1])) + 1;
    ret->height = toolkit_maths_max(2, this->height(ret->childs[0]), this->height(ret->childs[1])) + 1;
    return ret;
}

AVLNode * AVLObject::rotate_R(AVLNode * node)
{
    COMM_ASSERT_RETURN(node, NULL);

    COMM_ASSERT_RETURN(node->childs[0], node);  // Has Left Child

    AVLNode * ret = node->childs[0];
    node->childs[0] = ret->childs[1];
    ret->childs[1] = node;

    node->height = toolkit_maths_max(2, this->height(node->childs[0]), this->height(node->childs[1])) + 1;
    ret->height = toolkit_maths_max(2, this->height(ret->childs[0]), this->height(ret->childs[1])) + 1;
    return ret;
}

AVLNode * AVLObject::rotate_LR(AVLNode * node)
{
    COMM_ASSERT_RETURN(node, NULL);

    node->childs[0] = this->rotate_L(node->childs[0]);

    return this->rotate_R(node);
}

AVLNode * AVLObject::rotate_RL(AVLNode * node)
{
    COMM_ASSERT_RETURN(node, NULL);

    node->childs[1] = this->rotate_R(node->childs[1]);

    return this->rotate_L(node);
}

