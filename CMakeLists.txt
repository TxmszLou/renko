cmake_minimum_required(VERSION "3.13")

project("RenkoTest" VERSION 0.1.0 LANGUAGES C CXX)

# include(CTest)
# enable_testing()

# set(SOURCES test.cpp ./include/renko/core.h ./include/renko/particle.h ./include/renko/precision.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
file(GLOB_RECURSE MY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable("RenkoTest" "${MY_SOURCES}")
target_include_directories("RenkoTest" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

project("RenkoGL" VERSION 0.1.0 LANGUAGES CXX)
add_executable("RenkoGL" "tests/gl.cpp")
target_link_libraries("RenkoGL" GL GLU glut)
