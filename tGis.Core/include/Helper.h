#pragma once

#ifndef __HELPER_H__
#define __HELPER_H__

#pragma warning( disable: 4251 )

//namespace helper

#define BEGIN_NAME_SPACE(n1,n2) namespace n1 { namespace n2 {

#define END_NAME_SPACE(n1,n2) } }



//exports helper

#if defined(_MSC_VER) || defined(__MINGW32__)
#define TGIS_SYMBOL_IMPORT __declspec(dllimport)
#define TGIS_SYMBOL_EXPORT __declspec(dllexport)
#else
#if __GNUC__ >= 4
#define TGIS_SYMBOL_IMPORT __attribute__ ((visibility ("default")))
#define TGIS_SYMBOL_EXPORT __attribute__ ((visibility ("default")))
#else
#define TGIS_SYMBOL_IMPORT __attribute__((dllimport))
#define TGIS_SYMBOL_EXPORT __attribute__((dllexport))
#endif
#endif

#ifdef TGIS_EXPORTS
#define TGIS_API TGIS_SYMBOL_EXPORT
#else
#define TGIS_API TGIS_SYMBOL_IMPORT
#endif

#ifdef TGIS_UTILITY_EXPORTS
#define TGIS_UTILITY_API TGIS_SYMBOL_EXPORT
#else
#define TGIS_UTILITY_API TGIS_SYMBOL_IMPORT
#endif

#ifdef TGIS_SEGMENT_EXPORTS
#define TGIS_SEGMENT_API TGIS_SYMBOL_EXPORT
#else
#define TGIS_SEGMENT_API TGIS_SYMBOL_IMPORT
#endif


#if defined(_MSC_VER) || defined(__MINGW32__)

#include <Windows.h>
#include <direct.h>
#include <stdlib.h>
#include <io.h>

#define _tgis_max max

#define TGIS_MAX_PATH _MAX_PATH 

#define TGIS_PATH_SEPARATOR_CHAR  '\\'

#define TGIS_PATH_SEPARATOR_STR  "\\"

#define TGIS_EXT_SEPARATOR_CHAR  '.'

#define TGIS_EXT_SEPARATOR_STR  "."

#define _tgis_getcwd  _getcwd

#define _tgis_finddata_t _finddata_t

#define _tgis_findfirst _findfirst

#define _tgis_findnext _findnext

#define _tgis_findclose _findclose
 
#define _TGIS_A_HIDDEN _A_HIDDEN

#define _TGIS_A_SYSTEM _A_SYSTEM

#define _TGIS_A_SUBDIR _A_SUBDIR

#define _TGIS_A_RDONLY _A_RDONLY



#define _tgis_access _access

#define _TGIS_A_EXIST 0

#define _TGIS_A_WRITE 2

#define _TGIS_A_READ  4

#define _TGIS_A_READ_WRITE  6

#define _TGIS_R_OK 0

#endif




//custom method

#include <math.h>
#include <float.h>
#include <string>
#include <vector>
#include <time.h>


#define _tgis_isinf(a)   ((_fpclass(a) == _FPCLASS_PINF) || (_fpclass(a) == _FPCLASS_NINF))
#define _tgis_isnan(x)   _isnan(x)

inline double _tgis_round(double val, int places) {
	double t;
	double f = pow(10.0, (double)places);
	double x = val * f;

	if (_tgis_isinf(x) || _tgis_isinf(x)) {
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

inline void _tgis_localtime(struct tm * const tms,const time_t *tmt)
{
	localtime_s(tms, tmt);
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

TGIS_API int _tgis_find_first_of(const char* s, const char* m, int offset);

TGIS_API int _tgis_find_last_of(const char* s, const char* m, int offset);


// this shit for shit

template<class T>
struct _tGisObjectDestructor
{
	_tGisObjectDestructor(T* ptr)
	{
		_ptr = ptr;
	}

	~_tGisObjectDestructor()
	{
		delete _ptr;
	}

	T* _ptr;
};


#endif