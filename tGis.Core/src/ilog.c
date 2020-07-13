#include "ilog.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <direct.h>
#include "elr_mtx.h"

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
	elr_mtx mtx;
}
_loger_t;


TGIS_CORE_API loger_t  _init_log(const char *path, int level, int console)
{
	_loger_t *loger = (_loger_t*)malloc(sizeof(_loger_t));
	struct tm localtm;
	char   date[11];
	time_t now;

	if (loger != NULL)
	{
		now = time(NULL);
		memset(loger->path, '\0', MAX_PATH);
		memcpy(&localtm, localtime(&now), sizeof(struct tm));

		loger->level = level;
		loger->console = console;

		sprintf(date, "%4d-%02d-%02d", 1900 + localtm.tm_year, localtm.tm_mon+1, localtm.tm_mday);
		sprintf(loger->path, "%s%s.log.txt", path, date);
		loger->pdate = strstr(loger->path, date);

		if (elr_mtx_init(&loger->mtx) == 0)
		{
			free(loger);
			return NULL;
		}

		loger->fp = fopen(loger->path, "a+");
	}

	return loger;
}

TGIS_CORE_API void  _print_log(loger_t loger, int level, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	_vprint_log(loger, level, format, args);
	va_end(args);
}

TGIS_CORE_API void _vprint_log(loger_t loger, int level, const char * format, va_list args)
{
	_loger_t* log = (_loger_t*)loger;
	struct tm ltnow;
	char   date[11];
	time_t now;

	if (log == NULL || log->level < level)
		return;

	now = time(NULL);
	memcpy(&ltnow, localtime(&now), sizeof(struct tm));
	sprintf(date, "%4d-%02d-%02d", 1900 + ltnow.tm_year, ltnow.tm_mon+1, ltnow.tm_mday);

	if (strncmp(log->pdate, date, 10) != 0)
	{
		memcpy(log->pdate, date, 10);
		if (log->fp != NULL)
			fclose(log->fp);
		log->fp = fopen(log->path, "a+");
	}

	elr_mtx_lock(&log->mtx);
	if (log->console)
	{
		printf("[%s %02d:%02d:%02d %s]\n", date, ltnow.tm_hour, ltnow.tm_min, ltnow.tm_sec, LOG_LEVEL[level]);
		vprintf(format, args);
		puts("\n");
	}

	if (log->fp != NULL)
	{
		fprintf(log->fp, "[%s %02d:%02d:%02d %s]\n", date, ltnow.tm_hour, ltnow.tm_min, ltnow.tm_sec, LOG_LEVEL[level]);
		vfprintf(log->fp, format, args);
		fputs("\n\n", log->fp);
	}
	elr_mtx_unlock(&log->mtx);
}

TGIS_CORE_API void  _finalize_log(loger_t loger)
{
	_loger_t* log = (_loger_t*)loger;

	if (loger != NULL)
	{
		elr_mtx_finalize(&log->mtx);
		if (log->fp != NULL)
			fclose(log->fp);
		free(loger);
	}
}

TGIS_CORE_API void _flush_log(loger_t loger)
{
	_loger_t* log = (_loger_t*)loger;

	if (loger != NULL && log->fp != NULL)
	{
		fflush(log->fp);
	}
}




