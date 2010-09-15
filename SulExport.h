#ifndef __SulExport_H__
#define __SulExport_H__

/*
#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#  if defined( SIGMAUTIL_LIBRARY_STATIC )
#    define SUL_EXPORT
#  elif defined( SIGMAUTIL_LIBRARY_DYNAMIC )
#    define SUL_EXPORT   __declspec(dllexport)
#  else
#    define SUL_EXPORT   __declspec(dllimport)
#  endif
#else
#  define SUL_EXPORT
#endif
*/


#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#  if defined( SIGMAUTIL_LIBRARY_DYNAMICIMPORT )
#    define SUL_EXPORT   __declspec(dllimport)
#  elif defined( SIGMAUTIL_LIBRARY_DYNAMIC )
#    define SUL_EXPORT   __declspec(dllexport)
#  else
#    define SUL_EXPORT
#  endif
#else
#  define SUL_EXPORT
#endif



#endif // __SulExport_H__
