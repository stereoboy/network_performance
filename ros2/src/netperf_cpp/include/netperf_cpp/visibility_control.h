#ifndef NETPERF_CPP__VISIBILITY_CONTROL_H_
#define NETPERF_CPP__VISIBILITY_CONTROL_H_

#ifdef __cplusplus
extern "C"
{
#endif

// This logic was borrowed (then namespaced) from the examples on the gcc wiki:
//     https://gcc.gnu.org/wiki/Visibility

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define NETPERF_CPP_EXPORT __attribute__ ((dllexport))
    #define NETPERF_CPP_IMPORT __attribute__ ((dllimport))
  #else
    #define NETPERF_CPP_EXPORT __declspec(dllexport)
    #define NETPERF_CPP_IMPORT __declspec(dllimport)
  #endif
  #ifdef NETPERF_CPP_BUILDING_DLL
    #define NETPERF_CPP_PUBLIC NETPERF_CPP_EXPORT
  #else
    #define NETPERF_CPP_PUBLIC NETPERF_CPP_IMPORT
  #endif
  #define NETPERF_CPP_PUBLIC_TYPE NETPERF_CPP_PUBLIC
  #define NETPERF_CPP_LOCAL
#else
  #define NETPERF_CPP_EXPORT __attribute__ ((visibility("default")))
  #define NETPERF_CPP_IMPORT
  #if __GNUC__ >= 4
    #define NETPERF_CPP_PUBLIC __attribute__ ((visibility("default")))
    #define NETPERF_CPP_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define NETPERF_CPP_PUBLIC
    #define NETPERF_CPP_LOCAL
  #endif
  #define NETPERF_CPP_PUBLIC_TYPE
#endif

#ifdef __cplusplus
}
#endif

#endif  // NETPERF_CPP__VISIBILITY_CONTROL_H_
