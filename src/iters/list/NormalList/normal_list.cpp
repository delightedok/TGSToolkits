#include "../../../comms/comm_headers.h"
#include <iters/list/normal_list.h>

#define THIS_FILE "normal_list.cpp"
#define LOG_TAG "ITERS-LIST-NORMAL"

TGSTK_EXPORT NormalListObject::NormalListObject(ListVTable & mVTable): ListObject(mVTable)
{
    /* A Ring List */
    this->mDatas.next = &this->mDatas;
    this->mDatas.prev = &this->mDatas;
}

TGSTK_EXPORT NormalListObject::~NormalListObject(void)
{
    this->clear();
}

TGSTK_EXPORT int NormalListObject::iterate(Func_itersListOnIterate onIter, void * arg, int unwind)
{
    int ret = 0;
    ListNode * p = NULL;
    if (unwind)
    {
        p = mDatas.prev;
        while (p != &mDatas && !ret)
        {
            ret = onIter(p->ptr.ptr, arg);
            p = p->prev;
        }
    } else
    {
        p = mDatas.next;
        while (p != &mDatas && !ret)
        {
            ret = onIter(p->ptr.ptr, arg);
            p = p->next;
        }
    }
    return ret;
}

TGSTK_EXPORT int NormalListObject::rpush(void * obj, int size)
{
    int ret = -1;
    do {
        ListNode * pNode = (ListNode *)mmalloc(sizeof(ListNode));
        if (!pNode)
        {
            mlog_e(LOG_TAG, THIS_FILE, "Failed to alloc memory for a New List Node.");
            break;
        }
        pNode->ptr.ptr = this->doDuplicate(obj);
        pNode->ptr.size = size;
        pNode->next = &mDatas;
        pNode->prev = mDatas.prev;
        mDatas.prev->next = pNode;
        mDatas.prev = pNode;
        mNdatas++;
        ret = 0;
    } while (0);
    return ret;
}

TGSTK_EXPORT int NormalListObject::lpush(void * obj, int size)
{
    int ret = -1;
    do {
        ListNode * pNode = (ListNode *)mmalloc(sizeof(ListNode));
        if (!pNode)
        {
            mlog_e(LOG_TAG, THIS_FILE, "Failed to alloc memory for a New List Node.");
            break;
        }
        pNode->ptr.ptr = this->doDuplicate(obj);
        pNode->ptr.size = size;
        pNode->next = mDatas.next;
        pNode->prev = &mDatas;
        mDatas.next->prev = pNode;
        mDatas.next = pNode;
        mNdatas++;
        ret = 0;
    } while (0);
    return ret;
}

TGSTK_EXPORT void * NormalListObject::rpop(void)
{
    void * ret = NULL;
    do {
        ListNode * pNode = mDatas.prev;
        if (pNode == &mDatas)  // there is NO data in list
        {
            mNdatas = 0;  // to ensure the <field>mNdatas</field> is correct
            break;
        }
        pNode->prev->next = &mDatas;
        mDatas.prev = pNode->prev;
        mNdatas--;
        ret = pNode->ptr.ptr;
        mfree(pNode);
    } while (0);
    return ret;
}

TGSTK_EXPORT void * NormalListObject::lpop(void)
{
    void * ret = NULL;
    do {
        ListNode * pNode = mDatas.next;
        if (pNode == &mDatas)  // there is NO data in list
        {
            mNdatas = 0;  // to ensure the <field>mNdatas</field> is correct
            break;
        }
        pNode->next->prev = &mDatas;
        mDatas.next = pNode->next;
        mNdatas--;
        ret = pNode->ptr.ptr;
        mfree(pNode);
    } while (0);
    return ret;
}

TGSTK_EXPORT int NormalListObject::clear(void)
{
    while (this->mNdatas > 0)
    {
        void * data = this->rpop();
        this->doFree(data);
    }
    return 0;
}

