#include "comm_headers.h"
#include "comm_mems.h"
#include "comm_logger.h"

#define THIS_FILE "comm_mems.cpp"
#define LOG_TAG "COMMS-MEMS"

void * _comm_mems_alloc(unsigned int size)
{
    return new char[size];
}

void _comm_mems_free(void ** p)
{
    COMM_ASSERT_RETURN(p && *p, ;);
    char * _p = (char *)*p;
    delete _p;
    *p = NULL;
}

