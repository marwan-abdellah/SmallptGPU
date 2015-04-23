IF(APPLE)
    # Base directory
    INCLUDE_DIRECTORIES ("/System/Library/Frameworks")

    # Find the GLUT library
    FIND_LIBRARY(GLUT_LIBRARY GLUT REQUIRED)

    # Advanced variable
    MARK_AS_ADVANCED(GLUT_LIBRARY)

    # Adding the include directory to the systems
    SET(GLUT_INCLUDE_DIR "/usr/X11R6/include/")
    INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})

    # Adding the library directories
    SET(GLUT_LIB_DIR "/usr/X11R6/lib")
    LINK_DIRECTORIES(${GLUT_LIB_DIR})

    # Checking if GLUT was found or not
    IF(NOT GLUT_LIBRARY STREQUAL "")
        MESSAGE(STATUS "GLUT Found")
        MESSAGE("-- GLUT include directory : ${GLUT_INCLUDE_DIR}")
        MESSAGE("-- GLUT library directory : ${GLUT_LIBRARY}")
    ELSE(NOT GLUT_LIBRARY STREQUAL "")
        MESSAGE(FATAL_ERROR "GLUT NOT Found")
    ENDIF(NOT GLUT_LIBRARY STREQUAL "")

    # Linking against GLUT
    LINK_LIBRARIES(${GLUT_LIBRARY})
ELSE(APPLE)
    # Linux
    FIND_PACKAGE(GLUT REQUIRED)
    IF(GLUT_FOUND)
        MESSAGE(STATUS "GLUT Found")
        MESSAGE("-- GLUT include directory : ${GLUT_INCLUDE_DIR}")
        MESSAGE("-- GLUT library directory: ${GLUT_LIBRARY}")
        INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR}/GL)
        LINK_LIBRARIES(${GLUT_LIBRARY})
    ELSE(GLUT_FOUND)
        MESSAGE(FATAL_ERROR "GLUT NOT Found")
    ENDIF(GLUT_FOUND)
ENDIF(APPLE)




