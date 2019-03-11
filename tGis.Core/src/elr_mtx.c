#include "elr_mtx.h"

#if defined(_MSC_VER) || defined(__MINGW32__)

TGIS_CORE_API elr_counter_t elr_atomic_inc(elr_atomic_t* v)
{
	return InterlockedIncrement(v);
}

TGIS_CORE_API elr_counter_t elr_atomic_dec(elr_atomic_t* v)
{
	return InterlockedDecrement(v);
}

/*
** 初始化互斥体，返回0表示初始化失败
*/
TGIS_CORE_API int  elr_mtx_init(elr_mtx *mtx)
{
	DWORD err;
	InitializeCriticalSection(&mtx->_cs);
	err = GetLastError();
	if (STATUS_NO_MEMORY == err)
	{
		DeleteCriticalSection(&mtx->_cs);
		return 0;
	}

	return 1;
}

TGIS_CORE_API void elr_mtx_lock (elr_mtx *mtx)
{
	EnterCriticalSection(&mtx->_cs);
}

TGIS_CORE_API void elr_mtx_unlock(elr_mtx *mtx)
{
	LeaveCriticalSection(&mtx->_cs);
}

TGIS_CORE_API void elr_mtx_finalize(elr_mtx *mtx)
{
	DeleteCriticalSection(&mtx->_cs);
}
#endif