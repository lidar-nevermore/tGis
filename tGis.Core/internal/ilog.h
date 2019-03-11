
#ifndef __ILOG_H__
#define __ILOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ENABLE_ILOG

#define initial_log(path, level, console)         _initial_log(path, level, console)
#define print_log(loger, level, format, ...)      _print_log(loger, level, format, ##__VA_ARGS__)
#define finalize_log(loger)                       _finalize_log(loger)
#define flush_log(loger)                          _flush_log(loger)

#else

#define initial_log(path, level, console)         NULL
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

    loger_t  _initial_log(const char *path, int level, int console);

    void  _print_log(loger_t loger,int level, const char *format, ...);

	void  _flush_log(loger_t loger);

    void  _finalize_log(loger_t loger);

#ifdef __cplusplus
}  /* end of the 'extern "C"' block */
#endif

#endif



