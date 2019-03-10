
#ifndef __ILOG_H__
#define __ILOG_H__

#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <time.h>
#include <direct.h>

#include "iconf.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ENABLE_ILOG

#define initial_log(conf, name)                   _initial_log(conf,name)
#define print_log(loger, priority, format, ...)   _print_log(loger, priority, format, ##__VA_ARGS__)
#define finalize_log(loger)                       _finalize_log(loger)
#define flush_log(loger)                          _flush_log(loger)

#else

#define initial_log(conf, name)                   NULL
#define print_log(loger, priority, format, ...)
#define finalize_log(loger)
#define flush_log(loger)  

#endif

    enum log_level
    {
        LOG_EMERG,
        LOG_ALERT,
        LOG_CRIT,
        LOG_ERR,
        LOG_WARNING,
        LOG_NOTICE,
        LOG_INFO,
        LOG_DEBUG,
    };

    typedef void* loger_t;

    loger_t  _initial_log(const char *path,const char *name);

    void  _print_log(loger_t loger,int priority, const char *format, ...);

    void  _finalize_log(loger_t loger);

#ifdef __cplusplus
}  /* end of the 'extern "C"' block */
#endif

#endif



