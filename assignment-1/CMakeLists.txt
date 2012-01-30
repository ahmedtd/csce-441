
cmake_minimum_required (VERSION 2.6)
project (assignment-1)

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

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
	${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        @ONLY)
    add_custom_target(
        doc
        ${DOXYGEN_EXECUTABLE}
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
endif(DOXYGEN_FOUND)

configure_file (${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

add_executable (paint paint.cpp brush.cpp color.cpp vec2.cpp)
target_link_libraries (paint ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

