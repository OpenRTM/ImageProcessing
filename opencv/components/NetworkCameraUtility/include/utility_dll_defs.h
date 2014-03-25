
#ifndef NETWORKCAMERAUTILITY_API_H
#define NETWORKCAMERAUTILITY_API_H

#ifdef NetworkCameraUtility_BUILT_AS_STATIC
#  define NETWORKCAMERAUTILITY_API
#  define NETWORKCAMERAUTILITY_NO_EXPORT
#else
#  ifndef NETWORKCAMERAUTILITY_API
#    ifdef NetworkCameraUtility_EXPORTS
        /* We are building this library */
#      define NETWORKCAMERAUTILITY_API __declspec(dllexport)
#    else
        /* We are using this library */
#      define NETWORKCAMERAUTILITY_API __declspec(dllimport)
#    endif
#  endif

#  ifndef NETWORKCAMERAUTILITY_NO_EXPORT
#    define NETWORKCAMERAUTILITY_NO_EXPORT 
#  endif
#endif

#ifndef NETWORKCAMERAUTILITY_DEPRECATED
#  define NETWORKCAMERAUTILITY_DEPRECATED __declspec(deprecated)
#  define NETWORKCAMERAUTILITY_DEPRECATED_EXPORT NETWORKCAMERAUTILITY_API __declspec(deprecated)
#  define NETWORKCAMERAUTILITY_DEPRECATED_NO_EXPORT NETWORKCAMERAUTILITY_NO_EXPORT __declspec(deprecated)
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define NETWORKCAMERAUTILITY_NO_DEPRECATED
#endif

#endif
