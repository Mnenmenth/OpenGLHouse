cmake_minimum_required(VERSION 3.12)
project(OpenGLHouse)

set(CMAKE_CXX_STANDARD 14)

# Bundled library headers
include_directories(${CMAKE_SOURCE_DIR}/extlib)

# Get all .cpp and .h files within 'src' directory
file(GLOB_RECURSE SRC "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.h")
add_executable(OpenGLHouse ${SRC})


# Find and link OpenGL and GLUT
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(OpenGLHouse ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

# OSX Doesn't need GLEW and also has a different path to freeglut
if(NOT APPLE)
    find_package(GLEW REQUIRED)
    target_link_libraries(OpenGLHouse GLEW::GLEW)
    find_path(FREEGLUT_EXISTS GL/freeglut.h PATHS ${GLUT_INCLUDE_DIRS})
else()
    add_compile_definitions(OSX)
    find_path(FREEGLUT_EXISTS GLUT/freeglut.h PATHS ${GLUT_INCLUDE_DIRS})
endif(NOT APPLE)

# Add in FREEGLUT preprocessor definition if freeglut was found
if(FREEGLUT_EXISTS)
    add_compile_definitions(FREEGLUT)
    message(STATUS "freeglut.h found")
endif(FREEGLUT_EXISTS)

# Copy resources and shaders to build directory so they don't need to be
# manually copied every time they're changed
add_custom_target(COPY_SHADERS
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        ${CMAKE_BINARY_DIR}/shaders
        )
add_custom_target(COPY_RESOURCES
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        ${CMAKE_BINARY_DIR}/resources
        )
add_dependencies(OpenGLHouse COPY_SHADERS COPY_RESOURCES)