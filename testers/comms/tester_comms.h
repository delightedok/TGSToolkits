#ifndef __TESTER_COMMS_H_
#define __TESTER_COMMS_H_

#include <stdio.h>

#ifndef tester_logi
#define tester_logi(tag, file, fmt, args...) printf("[" tag "] " file "+%d | %s: " fmt "\n", __LINE__, __func__, ##args)
#endif

#ifndef tester_loge
#define tester_loge(tag, file, fmt, args...) printf("[" tag "] " file "+%d | %s: " fmt "\n", __LINE__, __func__, ##args)
#endif

#ifndef tester_logw
#define tester_logw(tag, file, fmt, args...) printf("[" tag "] " file "+%d | %s: " fmt "\n", __LINE__, __func__, ##args)
#endif

#ifndef tester_logd
#define tester_logd(tag, file, fmt, args...) printf("[" tag "] " file "+%d | %s: " fmt "\n", __LINE__, __func__, ##args)
#endif

#ifndef TESTER_ASSERT_RETURN
#define TESTER_ASSERT_RETURN(cond, retVal) do { if (!(cond)) return retVal; } while (0)
#endif

#endif