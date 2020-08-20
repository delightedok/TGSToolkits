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
            ret = this->push(&_root->childs[1], data, size);
        } else
        {
            ret = this->push(&_root->childs[0], data, size);
        }
    } while (0);
    return ret;
}

TGSTK_EXPORT void ADTObject::pop(void * obj)
{
    this->mDatas = this->pop(this->mDatas, obj);
}

ADTNode * ADTObject::pop(ADTNode * root, void * obj)
{
    COMM_ASSERT_RETURN(root, NULL);

    ADTNode * ret = root;
    int rc = 0;
    ADTNode * p = NULL;
    if (NULL != root)
    {
        rc = this->doCompare(obj, root->ptr.ptr);
        if (0 == rc)  // Find the Node
        {
            ADTNode * minNode = NULL;
            ret = root;

            if (root->childs[0] && root->childs[1])  // Has Left and Right Children
            {
                minNode = this->find_min_node(root->childs[1]);
                COMM_ASSERT_RETURN(minNode, root);  // In fact, minNode is always NOT NULL
                memcpy(&root->ptr, &minNode->ptr, sizeof(minNode->ptr));
                root->childs[1] = this->pop(root->childs[1], minNode->ptr.ptr);  // Pop the Min Node in Right Child
            } else  // Only have Left or Right Child
            {
                ret = root->childs[0] ? root->childs[0] : root->childs[1];
                p = root;
            }
        } else if (rc > 0)  // Find the Node from the Right Child
        {
            root->childs[1] = this->pop(root->childs[1], obj);
        } else if (rc < 0)  // Find the Node from the Left Child
        {
            root->childs[0] = this->pop(root->childs[0], obj);
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

ADTNode * ADTObject::find_max_node(ADTNode * root)
{
    ADTNode * ret = root;
    do {
        if (NULL == root->childs[1])  // Does NOT have the Right Child, which means the root node is the max node
        {
            break;
        }
        ret = this->find_max_node(root->childs[1]);
    } while (0);
    return ret;
}

ADTNode * ADTObject::find_min_node(ADTNode * root)
{
    ADTNode * ret = root;
    do {
        if (NULL == root->childs[0])  // Does NOT have the Left Child, which means the root node is the min node
        {
            break;
        }
        ret = this->find_min_node(root->childs[1]);
    } while (0);
    return ret;
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

