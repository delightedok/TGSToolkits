#ifndef __COMMS_COMM_LOGGER_H_
#define __COMMS_COMM_LOGGER_H_

enum CommLoggerType
{
    emCommLoggerStd = 1,  // display in shell
    emCommLoggerFile = 2,  // display in file
};

void _comm_logger_func(int level, const char * fmt, ...);

void _comm_logger_set_type(unsigned int type, const char * file);

void _comm_logger_set_level(unsigned int level);

#ifndef mlog_i
#define mlog_i(tag, file, f, a...) _comm_logger_func(50, "[" tag "] " file "+%d" " | %s: " f, __LINE__, __func__, ##a)
#endif

#ifndef mlog_e
#define mlog_e(tag, file, f, a...) _comm_logger_func(40, "[" tag "] " file "+%d" " | %s: " f, __LINE__, __func__, ##a)
#endif

#ifndef mlog_w
#define mlog_w(tag, file, f, a...) _comm_logger_func(30, "[" tag "] " file "+%d" " | %s: " f, __LINE__, __func__, ##a)
#endif

#ifndef mlog_d
#define mlog_d(tag, file, f, a...) _comm_logger_func(20, "[" tag "] " file "+%d" " | %s: " f, __LINE__, __func__, ##a)
#endif

#ifndef mlog_v
#define mlog_v(tag, file, f, a...) _comm_logger_func(10, "[" tag "] " file "+%d" " | %s: " f, __LINE__, __func__, ##a)
#endif

#ifndef mlog_set_type
#define mlog_set_type(type, file) _comm_logger_set_type(type, file)
#endif

#ifndef mlog_set_level
#define mlog_set_level(level) _comm_logger_set_level(level)
#endif

#endif