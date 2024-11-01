#ifndef _CLOCK_TIMER_H
#define _CLOCK_TIMER_H

#ifdef _WIN32

#include <windows.h>

#define GET_TIME(now) { \
   LARGE_INTEGER frequency; \
   LARGE_INTEGER time; \
   QueryPerformanceFrequency(&frequency); \
   QueryPerformanceCounter(&time); \
   now = (double)time.QuadPart / (double)frequency.QuadPart; \
}

#else  // UNIX

#include <sys/time.h>
#include <time.h>
#define BILLION 1000000000L

/* The argument now should be a double (not a pointer to a double) */
#define GET_TIME(now) { \
   struct timespec time; \
   clock_gettime(CLOCK_MONOTONIC, &time); \
   now = time.tv_sec + time.tv_nsec/1000000000.0; \
}


#endif

#endif