#ifndef LOGGING_
#define LOGGING_

#include <cstdio>
using namespace std;

#define ERROR       (300)
#define WARNING     (200)
#define INFO        (100)

#define LOG_ERROR(S, ...) 
#define LOG_WARNING(S, ...) 
#define LOG_INFO(S, ...) 

#ifdef DEBUG

#if ERROR >= DEBUG
#undef  LOG_ERROR
#define LOG_ERROR(S, ...) fprintf(stderr, S, __VA_ARGS__)
#endif

#if WARNING >= DEBUG
#undef  LOG_WARNING
#define LOG_WARNING(S, ...) fprintf(stderr, S, __VA_ARGS__)
#endif

#if INFO >= DEBUG
#undef  LOG_INFO
#define LOG_INFO(S, ...) fprintf(stderr, S, __VA_ARGS__)
#endif

#endif

#endif
