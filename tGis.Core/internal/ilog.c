#include "ilog.h"
#include "iconf.h"


#ifndef MAX_PATH
#define MAX_PATH 260
#endif


const char* LOG_LEVEL[8] =
{
	"EMERG",
	"ALERT",
	"CRIT",
	"ERR",
	"WARNING",
	"NOTICE",
	"INFO",
	"DEBUG"
};

typedef struct __loger_t
{
	int level;
	int console;
	char *pdate;
	char path[MAX_PATH];
	FILE* fp;
}
_loger_t;

int get_log_level(const char* level)
{
	int i = 0;

	if (level == NULL) return 7;

	for (; i < 8; i++)
	{
		if (_stricmp(level, LOG_LEVEL[i]) == 0)
			return i;
	}

	return 7;
}

int get_log_console(const char* console)
{
	if (console == NULL) return 0;

	return _stricmp(console, "yes") == 0 ? 1 : 0;
}

loger_t  _initial_log(const char *path, const char *name)
{
	conf_session_t session;
	_loger_t *loger = (_loger_t*)malloc(sizeof(_loger_t));
	struct tm localtm;
	char   date[11];
	time_t now;

	if (loger != NULL)
	{
		now = time(NULL);
		memset(loger->path, '\0', MAX_PATH);
		session.name = name;
		memcpy(&localtm, localtime(&now), sizeof(struct tm));

		if (read_conf_session(path, &session))
		{
			loger->level = get_log_level(get_conf_str(&session, "level"));
			loger->console = get_log_console(get_conf_str(&session, "console"));

			sprintf(date, "%4d-%02d-%02d", 1900 + localtm.tm_year, localtm.tm_mon, localtm.tm_mday);
			_getcwd(loger->path, MAX_PATH);
			sprintf(loger->path, "%s\\%s%s.log", loger->path, get_conf_str(&session, "dst"), date);
			loger->pdate = strstr(loger->path, date);
			loger->fp = fopen(loger->path, "a+");
		}
		else
		{
		    free(loger);
			loger = NULL;
		}

		free_conf_session(&session);
	}

	return loger;
}

void  _print_log(loger_t loger, int priority, const char *format, ...)
{
	va_list args;
	_loger_t* log = (_loger_t*)loger;
	struct tm ltnow;
	char   date[11];
	time_t now = time(NULL);

	memcpy(&ltnow, localtime(&now), sizeof(struct tm));

	if (log == NULL || log->level < priority)
		return;

	sprintf(date, "%4d-%02d-%02d", 1900 + ltnow.tm_year, ltnow.tm_mon, ltnow.tm_mday);

	if (strncmp(log->pdate, date, 10) != 0)
	{
		memcpy(log->pdate, date, 10);
		if (log->fp != NULL)
			fclose(log->fp);
		log->fp = fopen(log->path, "a+");
	}

	va_start(args, format);

	if (log->console)
	{
		printf("[%s %02d:%02d:%02d %s]\n", date, ltnow.tm_hour, ltnow.tm_min, ltnow.tm_sec, LOG_LEVEL[priority]);
		vprintf(format, args);
		puts("\n");
	}

	if (log->fp != NULL)
	{
		fprintf(log->fp, "[%s %02d:%02d:%02d %s]\n", date, ltnow.tm_hour, ltnow.tm_min, ltnow.tm_sec, LOG_LEVEL[priority]);
		vfprintf(log->fp, format, args);
		fputs("\n\n", log->fp);
	}

	va_end(args);
}

void  _finalize_log(loger_t loger)
{
	if (loger != NULL)
	{
		if (((_loger_t*)loger)->fp != NULL)
			fclose(((_loger_t*)loger)->fp);
		free(loger);
	}
}

void _flush_log(loger_t loger)
{
	if (loger != NULL && ((_loger_t*)loger)->fp != NULL)
	{
		fflush(((_loger_t*)loger)->fp);
	}
}




