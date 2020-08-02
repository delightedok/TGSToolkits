#include "comm_headers.h"
#include "comm_logger.h"
#include <mutex>
#include <stdarg.h>

using namespace std;

#define THIS_FILE "comm_logger.cpp"
#define LOG_TAG "COMMS-LOGGER"

#define COMM_LOGGER_DEFAULT_BUFFER_SIZE 2048

#define COMM_LOGGER_TIME_FORMAT "%04d-%02d-%02d %02d:%02d:%02d "
#define COMM_LOGGER_TIME_ARGS(T) T.tm_year + 1900, T.tm_mon + 1, T.tm_mday, T.tm_hour, T.tm_min, T.tm_sec

class CommLoggerObject
{
public:
    static CommLoggerObject * get_instance(void);
    static void del_instance(void);
public:
    void vlogout(int level, const char * fmt, va_list ap);
    void set_logout_level(int level);
    void set_logout_type(unsigned int type, const char * file);
private:
    CommLoggerObject(void);
    ~CommLoggerObject(void);
private:
    static CommLoggerObject * mIns;  // singleton
    static mutex mLocker;  // mutex for <func>get_instance</func> and <func>del_instance</func>
private:
    int mCurLv;  // current logout level
    unsigned int mType;  // display type, look up in <enum>CommLoggerType</enum>
    FILE * mFp;  // Handle of logging file
    unsigned int mNlines;  // how many lines that have logging out to file but NOT sync
    mutex mLocker2;  // mutex for logging
};

CommLoggerObject * CommLoggerObject::mIns = NULL;
mutex CommLoggerObject::mLocker;

CommLoggerObject * CommLoggerObject::get_instance(void)
{
    if (!CommLoggerObject::mIns)
    {
        COMM_LOCK(CommLoggerObject::mLocker);
        if (!CommLoggerObject::mIns)
        {
            CommLoggerObject::mIns = new CommLoggerObject;
        }
        COMM_UNLOCK(CommLoggerObject::mLocker);
    }
    return CommLoggerObject::mIns;
}

void CommLoggerObject::del_instance(void)
{
    if (CommLoggerObject::mIns)
    {
        COMM_LOCK(CommLoggerObject::mLocker);
        if (CommLoggerObject::mIns)
        {
            delete CommLoggerObject::mIns;
            CommLoggerObject::mIns = NULL;
        }
        COMM_UNLOCK(CommLoggerObject::mLocker);
    }
}

CommLoggerObject::CommLoggerObject()
{
    this->mCurLv = 0;
    this->mType = emCommLoggerStd;
    this->mFp = NULL;
    this->mNlines = 0;
}

CommLoggerObject::~CommLoggerObject(void)
{
    this->mCurLv = 0;
    this->mType = emCommLoggerStd;
    COMM_LOCK(this->mLocker2);
    if (this->mFp)
    {
        fclose(this->mFp);
        this->mFp = NULL;
    }
    COMM_UNLOCK(this->mLocker2);
    this->mNlines = 0;
}

void CommLoggerObject::vlogout(int level, const char * fmt, va_list ap)
{
    COMM_ASSERT_RETURN(level >= this->mCurLv && fmt, ;);

    char vbuf[COMM_LOGGER_DEFAULT_BUFFER_SIZE] = {0};
    time_t t;
    struct tm T;

    vsnprintf(vbuf, sizeof(vbuf), fmt, ap);

    t = time(NULL);
    localtime_r(&t, &T);

    if (emCommLoggerStd & this->mType)
    {
        fprintf(stderr, COMM_LOGGER_TIME_FORMAT "%s", COMM_LOGGER_TIME_ARGS(T), vbuf);
    }

    if ((emCommLoggerFile & this->mType))
    {
        int doSync = 0;
        COMM_LOCK(this->mLocker2);
        if (this->mFp)
        {
            fprintf(mFp, COMM_LOGGER_TIME_FORMAT "%s", COMM_LOGGER_TIME_ARGS(T), vbuf);
            this->mNlines++;
            if (this->mNlines >= 50)  // when logging out 50 lines, force sync
            {
                this->mNlines = 0;
                doSync = 1;
            }
        }
        if (doSync)
        {
            fsync(fileno(this->mFp));
        }
        COMM_UNLOCK(this->mLocker2);
    }
}

void CommLoggerObject::set_logout_level(int level)
{
    this->mCurLv = level;
}

void CommLoggerObject::set_logout_type(unsigned int type, const char * file)
{
    COMM_LOCK(this->mLocker2);
    do {
        if ((type & emCommLoggerFile) && file)
        {
            if (!this->mFp)
            {
                this->mFp = fopen(file, "a");
                if (!this->mFp)
                {
                    mlog_e(LOG_TAG, THIS_FILE, "Failed to open logging file[%s], err: %d desc: %s.",
                        file, errno, strerror(errno));
                    type &= ~emCommLoggerFile;
                    break;
                }
            }
        } else
        {
            if (this->mFp)
            {
                fclose(this->mFp);
                this->mFp = NULL;
            }
        }
    } while (0);
    COMM_UNLOCK(this->mLocker2);
    this->mType = type;
}

void _comm_logger_func(int level, const char * fmt, ...)
{
    CommLoggerObject * ins = NULL;
    va_list ap;
    va_start(ap, fmt);
    ins = CommLoggerObject::get_instance();
    if (ins)
    {
        ins->vlogout(level, fmt, ap);
    }
    va_end(ap);
}

void _comm_logger_set_type(unsigned int type, const char * file)
{
    CommLoggerObject * ins = NULL;
    ins = CommLoggerObject::get_instance();
    if (ins)
    {
        ins->set_logout_type(type, file);
    }
}

void _comm_logger_set_level(int level)
{
    CommLoggerObject * ins = NULL;
    ins = CommLoggerObject::get_instance();
    if (ins)
    {
        ins->set_logout_level(level);
    }
}
