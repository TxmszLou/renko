cmake_minimum_required(VERSION "3.13")
project(imgui)
# target_include_directories(imgui PUBLIC)

set(SOURCE_FILES imgui.cpp imgui_draw.cpp imgui_demo.cpp imgui_tables.cpp imgui_widgets.cpp imgui_impl_opengl3.cpp imgui_impl_glfw.cpp) # Add all .cpp files of project root directory as source files
add_library(imgui ${SOURCE_FILES})