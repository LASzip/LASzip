/* DLL export definitions for LASzip */

#ifndef LASZIPEXPORT_H
#define LASZIPEXPORT_H

#ifndef LASZIP_DLL
#if (defined(_MSC_VER) || defined __CYGWIN__) && !defined(LASZIP_DISABLE_DLL)
    #if defined(LASZIP_DLL_EXPORT)
    #   define LASZIP_DLL   __declspec(dllexport)
    #elif defined(LASZIP_DLL_IMPORT)
    #   define LASZIP_DLL   __declspec(dllimport)
    #else
    #   define LASZIP_DLL
    #endif
#else
    #  if defined(USE_GCC_VISIBILITY_FLAG)
    #    define LASZIP_DLL     __attribute__ ((visibility("default")))
    #  else
    #    define LASZIP_DLL
    #  endif
#endif
#endif


#endif
