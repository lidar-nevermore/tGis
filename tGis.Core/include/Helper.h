#pragma once

#ifndef __HELPER_H__
#define __HELPER_H__

#include "tGisCoreCfg.h"

#include <math.h>
#include <float.h>
#include <string>
#include <vector>
#include <time.h>

//portability helper
#if defined(_MSC_VER) || defined(__MINGW32__)

  #include <windows.h>
  #include <direct.h>
  #include <stdlib.h>
  #include <io.h>
  
  #define _tgis_max max
  

  #define TGIS_MAX_PATH _MAX_PATH 
  #define TGIS_PATH_SEPARATOR_CHAR  '\\'
  #define TGIS_PATH_SEPARATOR_STR  "\\"
  #define TGIS_EXT_SEPARATOR_CHAR  '.'
  #define TGIS_EXT_SEPARATOR_STR  "."

  TGIS_CORE_API void _tgis_get_exe_dir(char* dir, int* size);

  #define _TGIS_A_NORMAL _A_NORMAL
  #define _TGIS_A_HIDDEN _A_HIDDEN
  #define _TGIS_A_SYSTEM _A_SYSTEM
  #define _TGIS_A_RDONLY _A_RDONLY
  #define _TGIS_A_SUBDIR _A_SUBDIR
  typedef void(*_tgis_on_traverse)(void* usr, const char * dir, const char* name, unsigned int attrib);
  
  TGIS_CORE_API void _tgis_traverse_dir(const char* dir, const char* pat, void* usr, _tgis_on_traverse on_traverse);

  #define _TGIS_OK_EXIST    0  
  #define _TGIS_OK_WRITE    2  
  #define _TGIS_OK_READ     4  
  #define _TGIS_OK_ACCESS   0

  #define _tgis_access _access



  inline void _tgis_localtime(const time_t *tmt, struct tm * tms)
  {
	  localtime_s(tms, tmt);
  }

#endif


//common method


#define _tgis_isinf(a)   ((_fpclass(a) == _FPCLASS_PINF) || (_fpclass(a) == _FPCLASS_NINF))
#define _tgis_isnan(x)   _isnan(x)

inline double _tgis_round(double val, int places) {
	double t;
	double f = pow(10.0, (double)places);
	double x = val * f;

	if (_tgis_isinf(x) || _tgis_isnan(x)) {
		return val;
	}

	if (x >= 0.0) {
		t = ceil(x);
		if ((t - x) > 0.50000000001) {
			t -= 1.0;
		}
	}
	else {
		t = ceil(-x);
		if ((t + x) > 0.50000000001) {
			t -= 1.0;
		}
		t = -t;
	}
	x = t / f;

	return !_tgis_isnan(x) ? x : t;
}

inline void _tgis_str_split(char* str, const char *delim, std::vector<std::string>& out)
{
	char * p = std::strtok(str, delim);
	while (p != 0)
	{
		bool not_delim = true;
		const char* d = delim;
		while (*d != 0)
		{
			if (*p == *d)
			{
				not_delim = false;
				break;
			}
			d++;
		};
		if (not_delim)
			out.push_back(std::string(p));
		p = std::strtok(NULL, delim);
	}
}

TGIS_CORE_API int _tgis_find_first_of(const char* s, const char* m, int offset);

TGIS_CORE_API int _tgis_find_last_of(const char* s, const char* m, int offset);

TGIS_CORE_API char* _tgis_str_trim(char *src);
#endif