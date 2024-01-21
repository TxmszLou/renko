cmake_minimum_required(VERSION "3.13")

project("RenkoTest" VERSION 0.1.0 LANGUAGES C CXX)

# include(CTest)
# enable_testing()

# set(SOURCES test.cpp ./include/renko/core.h ./include/renko/particle.h ./include/renko/precision.h)
# find_package(GLEW REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
file(GLOB_RECURSE MY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable("RenkoTest" "${MY_SOURCES}")
target_include_directories("RenkoTest" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
add_subdirectory(vendor/glad)
add_subdirectory(vendor/stb_image)
target_include_directories("RenkoTest" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/vendor")
target_link_libraries("RenkoTest" PRIVATE glfw OpenGL::GL glad stb_image)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

project("RenkoGL" VERSION 0.1.0 LANGUAGES CXX)
add_executable("RenkoGL" "tests/gl.cpp")
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
target_link_libraries("RenkoGL" PRIVATE OpenGL::GL OpenGL::GLU GLUT::GLUT)
