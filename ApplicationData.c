#include "ApplicationData.h"
#include "ApplicationData.hpp"

void initializeApplicationData(void)
{
    // Initial window/image size
    g_windowWidth = 512;
    g_windowHeight = 512;

    // Don't print help
    g_printHelp = 0;
}
