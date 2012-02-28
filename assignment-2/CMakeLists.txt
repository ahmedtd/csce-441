
cmake_minimum_required (VERSION 2.6)
project (assignment-2)

#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall --pedantic -Wextra")

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

add_executable (scan-convert scan-convert.cpp color.cpp vec2.cpp)
target_link_libraries (scan-convert ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

