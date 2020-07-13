
#ifndef __ILOG_H__
#define __ILOG_H__

#include "tGis_CoreCfg.h"

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ENABLE_ILOG

#define init_log(path, level, console)            _init_log(path, level, console)
#define print_log(loger, level, format, ...)      _print_log(loger, level, format, ##__VA_ARGS__)
#define vprint_log(loger, level, format, args)    _vprint_log(loger, level, format, args)
#define finalize_log(loger)                       _finalize_log(loger)
#define flush_log(loger)                          _flush_log(loger)

#else

#define init_log(path, level, console)         NULL
#define print_log(loger, level, format, ...)
#define vprint_log(loger, level, format, args)
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

	TGIS_CORE_API loger_t  _init_log(const char *path, int level, int console);

	TGIS_CORE_API void  _print_log(loger_t loger,int level, const char *format, ...);

	TGIS_CORE_API void  _vprint_log(loger_t loger, int level, const char *format, va_list args);

	TGIS_CORE_API void  _flush_log(loger_t loger);

	TGIS_CORE_API void  _finalize_log(loger_t loger);

#ifdef __cplusplus
}  /* end of the 'extern "C"' block */
#endif

#endif



