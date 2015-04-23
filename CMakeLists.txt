CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

PROJECT( SmallPTGPU )

# Add the CMAKE directory into the source tree
SET( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )

# OpenCL
## TODO: Create a specific finder under the CMAKE directory
FIND_PACKAGE( OpenCL REQUIRED )

# OpenGL
# FIND_PACKAGE( OpenGL REQUIRED )
INCLUDE (FindOpenGL)

# GLEW
# FIND_PACKAGE( GLEW REQUIRED )
INCLUDE (FindGLEW)

# GLUT
# FIND_PACKAGE( GLUT REQUIRED )
INCLUDE (FindGLUT)

## TODO: Should be handled in the specific finder
INCLUDE_DIRECTORIES( ${OPENCL_INCLUDE_DIR} )
LINK_LIBRARIES(${OPENCL_LIBRARY})

SET( SRC_FILES 
	displayfunc.c
	vec.h 
	camera.h 
	geom.h 
	displayfunc.h 
	simplernd.h 
	scene.h 
	geomfunc.h
)

# Create the executable
ADD_EXECUTABLE( cpu smallptCPU.c ${SRC_FILES} )

ADD_EXECUTABLE( gpu smallptGPU.c ${SRC_FILES} )
