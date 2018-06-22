#pragma once

#ifndef __HELPER_H__
#define __HELPER_H__


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


#if defined(_MSC_VER) || defined(__MINGW32__)

#include <direct.h>
#include <stdlib.h>
#include <io.h>

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

#include "Event.h"

BEGIN_NAME_SPACE(tGis, Core)

struct TGIS_API Progress
{
	Progress(int value, int max = 100, const char* msg = nullptr)
		:Value(value)
		,Max(max)
		,Message(msg)
	{
	}

	const int Value;
	const int Max;
	const char* const Message;
};

typedef IEventHandler<Progress> ProgressEventHandler;
typedef Event<Progress> ProgressEvent;

template class TGIS_API IEventHandler<Progress>;
template class TGIS_API Event<Progress>;

#define my_isinf(a)   ((_fpclass(a) == _FPCLASS_PINF) || (_fpclass(a) == _FPCLASS_NINF))
#define my_isnan(x)   _isnan(x)

inline double _tgis_round(double val, int places) {
	double t;
	double f = pow(10.0, (double)places);
	double x = val * f;

	if (my_isinf(x) || my_isinf(x)) {
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

	return !my_isnan(x) ? x : t;
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
		if(not_delim)
			out.push_back(std::string(p));
		p = std::strtok(NULL, delim);
	}
}








// this shit for shit

template<class T>
struct PtrDestructor
{
	PtrDestructor(T* ptr)
	{
		_ptr = ptr;
	}

	~PtrDestructor()
	{
		delete _ptr;
	}

	T* _ptr;
};

END_NAME_SPACE(tGis, Core)













































// this shit for shit

#ifdef PERCEPTRON_EXPORTS

#define EXPORT_CLASS_ENABLE_DATA_STUB(CLASS_NAME) struct CLASS_NAME##DataStub


#define EXPORT_CLASS_DEC_CONSTRUCTOR(CLASS_NAME,...)\
CLASS_NAME(__VA_ARGS__);\
CLASS_NAME(CLASS_NAME##DataStub* ,##__VA_ARGS__)

#else

#define EXPORT_CLASS_ENABLE_DATA_STUB(CLASS_NAME)


#define EXPORT_CLASS_DEC_CONSTRUCTOR(CLASS_NAME,...)\
CLASS_NAME(__VA_ARGS__)

#endif



#define EXPORT_CLASS_IMP_CONSTRUCTOR(CLASS_NAME,...)\
CLASS_NAME##DataStub::CLASS_NAME##DataStub(__VA_ARGS__)\
:CLASS_NAME(this ,##__VA_ARGS__)\
{}\
CLASS_NAME::CLASS_NAME(__VA_ARGS__)\
{\
    ::new (this) CLASS_NAME##DataStub(__VA_ARGS__);\
}\
CLASS_NAME::CLASS_NAME(CLASS_NAME##DataStub*, ##__VA_ARGS__)


#define EXPORT_CLASS_DEC_ALLOCATOR(CLASS_NAME)\
static void* operator new(size_t size);\
static void operator delete(void *p)


#define EXPORT_CLASS_IMP_ALLOCATOR(CLASS_NAME)\
void* CLASS_NAME::operator new(size_t size)\
{\
    return malloc(sizeof(CLASS_NAME##DataStub));\
}\
void CLASS_NAME::operator delete(void *p)\
{\
	free(p);\
}


#define BEGIN_DATA_STUB(CLASS_NAME) \
struct CLASS_NAME##DataStub : protected CLASS_NAME {


#define END_DATA_STUB };


#define DATA_STUB_SUPPORT_CONSTRUCTOR(CLASS_NAME,...)\
CLASS_NAME##DataStub(__VA_ARGS__);


#define GET_DATA_STUB(CLASS_NAME) CLASS_NAME##DataStub* DATA_STUB = ((CLASS_NAME##DataStub*)this)

















#endif