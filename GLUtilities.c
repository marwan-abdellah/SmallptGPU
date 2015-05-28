#include "GLContext.h"

/**
 * @brief glPrintString
 * @param font
 * @param string
 */
void glPrintString(void *font, const char *string)
{
    const int stringLength = (int) strlen(string);
    int i = 0;
    for (i = 0; i < stringLength; i++)
        glutBitmapCharacter(font, string[i]);
}
