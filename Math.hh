#ifndef MATH_HH
#define MATH_HH

#ifndef GPU_KERNEL

// Clamping
#define clamp(x, lowLimit, highLimit)                                           \
    ((x) < (lowLimit) ? (lowLimit) : ((x) > (highLimit) ? (highLimit) : (x)))

// Maximum
#define max(val1, val2)                                                         \
    ((val1) > (val2) ? (val1) : (val2))

// Minimum
#define min(val1, val2)                                                         \
    ((val1) < (val2) ? (val1) : (val2))

#endif

// Convert to integer
#define toInt(x)                                                                \
    ((int)(pow(clamp(x, 0.f, 1.f), 1.f / 2.2f) * 255.f + .5f))

#endif // MATH_HH
