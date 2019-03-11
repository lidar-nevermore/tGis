#ifndef __TGIS_CORE_CFG_H__
#define __TGIS_CORE_CFG_H__

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

#ifdef TGIS_CORE_EXPORTS
  #define TGIS_CORE_API TGIS_SYMBOL_EXPORT
#else
  #define TGIS_CORE_API TGIS_SYMBOL_IMPORT
#endif

#endif
