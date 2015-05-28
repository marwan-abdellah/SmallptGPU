#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <math.h>

#if defined(__linux__) || defined(__APPLE__)
#include <sys/time.h>
#elif defined (WIN32)
#include <windows.h>
#else
Unsupported Platform !!!
#endif

#include "Profiler.h"

/**
 * @brief profileCurrentStep
 * Returns the time at this step in microsceonds
 * @return
 */
double profileCurrentStep()
{
#if defined(__linux__) || defined(__APPLE__)
    struct timeval t;
    gettimeofday(&t, NULL);

    return t.tv_sec + t.tv_usec / 1000000.0;
#elif defined (WIN32)
    return GetTickCount() / 1000.0;
#else
    printf("Unsupported Platform !!!");
#endif
}
