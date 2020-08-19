#include "comm_headers.h"
#include "comm_mems.h"
#include "comm_logger.h"
#include <new>

using namespace std;

#define THIS_FILE "comm_mems.cpp"
#define LOG_TAG "COMMS-MEMS"

void * _comm_mems_alloc(unsigned int size)
{
    void * ret = NULL;
    try {
        ret = new char[size];
    } catch (bad_alloc & e) {
        mlog_d(LOG_TAG, THIS_FILE, "Failed to alloc memory with size[%ld], desc: %s.", size, e.what());
        ret = NULL;
    }
    return ret;
}

void _comm_mems_free(void ** p)
{
    COMM_ASSERT_RETURN(p && *p, ;);
    char * _p = (char *)*p;
    delete _p;
    *p = NULL;
}

