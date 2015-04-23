IF( APPLE )
    # Frameworks base directory
    INCLUDE_DIRECTORIES( "/System/Library/Frameworks" )

    # Find the GLEW library
    FIND_LIBRARY( GLEW_LIBRARY GLEW REQUIRED )

    # Set it as an advanced variable
    MARK_AS_ADVANCED( GLEW_LIBRARY )

    # Adding the include directory to the systems
    SET( GLEW_INCLUDE_DIR "/opt/local/include/" )
    INCLUDE_DIRECTORIES( ${GLEW_INCLUDE_DIR} )

    # Adding the library directories
    SET(GLEW_LIB_DIR "/opt/local/lib")
    LINK_DIRECTORIES(${GLEW_LIB_DIR})

    # Checking if GLEW was found or not
    IF(NOT GLEW_LIBRARY STREQUAL "")
        MESSAGE(STATUS "GLEW Found")
        MESSAGE("-- GLEW include directory : ${GLEW_INCLUDE_DIR}")
        MESSAGE("-- GLEW library : ${GLEW_LIBRARY}")
    ELSE(NOT GLEW_LIBRARY STREQUAL "")
        MESSAGE(FATAL_ERROR "GLEW NOT Found")
    ENDIF(NOT GLEW_LIBRARY STREQUAL "")

    # Linking against GLEW
    LINK_LIBRARIES(${GLEW_LIBRARY})
ELSE(APPLE)
    # Basically, linux
    MESSAGE(CUDA_SDK_ROOT: ${CUDA_SDK_ROOT})
    FIND_PATH(GLEW_INCLUDE_DIR "glew.h"
      HINTS "${CUDA_SDK_ROOT}/C/common/inc/GL"
        /usr/include
        /usr/local/include
        /opt/local/include
    )

    FIND_LIBRARY(GLEW_LIBRARY NAMES GLEW_x86_64
      HINTS "${CUDA_SDK_ROOT}/C/common/lib/linux"
      PATHS
        /usr/lib
        /usr/local/lib
        /opt/local/lib
    )

    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLEW DEFAULT_MSG
                                      GLEW_LIBRARY
                                      GLEW_INCLUDE_DIR)

    IF(GLEW_FOUND)
        MESSAGE(STATUS "GLEW Found")
        MESSAGE("-- GLEW include directory : ${GLEW_INCLUDE_DIR}")
        MESSAGE("-- GLEW library directory : ${GLEW_LIBRARY}")
        INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR}/GL)
        LINK_LIBRARIES(${GLEW_LIBRARY})
    ELSE(GLEW_FOUND)
        MESSAGE(FATAL_ERROR "GLEW NOT Found")
    ENDIF(GLEW_FOUND)
ENDIF(APPLE)
