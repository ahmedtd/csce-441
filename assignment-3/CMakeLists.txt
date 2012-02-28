
cmake_minimum_required (VERSION 2.6)
project (assignment-3)

# Check the system for OpenGL
include (FindOpenGL)
if (OPENGL_FOUND)
   include_directories (${OPENGL_INCLUDE_DIR})
endif (OPENGL_FOUND)

# Check the system for GLUT
include (FindGLUT)
if (GLUT_FOUND)
   include_directories (${GLUT_INCLUDE_DIR})
endif (GLUT_FOUND)

add_executable (assignment3 assignment3.cpp init.cpp mat.cpp)
target_link_libraries (assignment3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

