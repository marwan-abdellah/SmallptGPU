#ifndef OPENGL_CONTEXT_H
#define	OPENGL_CONTEXT_H

#include <math.h>

#ifdef __MACOSX
#include <GLut/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Vector3.hh"

extern unsigned int renderingFlags;
extern char captionBuffer[256];


extern void initializeGLUT(int argc, char *argv[], char *windowTittle);
extern double profileCurrentStep();

extern void readScene(char *);
extern void updateCamera();

void glPrintString(void *font, const char *string);

void glKeyboard(unsigned char key, int x, int y);




#endif	// OPENGL_CONTEXT_H

