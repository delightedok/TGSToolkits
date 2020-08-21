#include "../comms/comm_headers.h"
#include <maths/maths.h>
#include <stdarg.h>


#define THIS_FILE "maths.cpp"
#define LOG_TAG "MATHS-MATHS"


TGSTK_EXPORT int toolkit_maths_min(unsigned int nums, ...)
{
    COMM_ASSERT_RETURN(nums > 0, nums);

    int ret = nums;
    unsigned int i = 0;
    va_list ap;
    va_start(ap, nums);
    for (i = 0; i < nums; i++)
    {
        if (0 == i)
        {
            ret = va_arg(ap, int);
        } else
        {
            int n = va_arg(ap, int);
            if (ret > n)
            {
                ret = n;
            }
        }
    }
    return ret;
}

TGSTK_EXPORT int toolkit_maths_max(unsigned int nums, ...)
{
    COMM_ASSERT_RETURN(nums > 0, nums);

    int ret = nums;
    unsigned int i = 0;
    va_list ap;
    va_start(ap, nums);
    for (i = 0; i < nums; i++)
    {
        if (0 == i)
        {
            ret = va_arg(ap, int);
        } else
        {
            int n = va_arg(ap, int);
            if (ret < n)
            {
                ret = n;
            }
        }
    }
    return ret;
}

