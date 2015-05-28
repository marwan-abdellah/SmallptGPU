#ifndef DEFINES_HH
#define DEFINES_HH

// Rendering defines
#define RFLAGS_DISABLE_DIFFUSE_PATH ( 1 )

#define HELP_FONT GLUT_BITMAP_HELVETICA_18

// Math defines
#define EPSILON ( 0.01f )
#define FLOAT_PI ( 3.14159265358979323846f )

// Hardware
typedef enum
{
    CPU,    // Implementation runs on the CPU
    GPU     // Implementation runs on the GPU
} HARDWARE;

#endif // DEFINES_HH
