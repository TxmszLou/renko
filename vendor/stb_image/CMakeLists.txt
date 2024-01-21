cmake_minimum_required(VERSION "3.13")
project(stb_image)

add_library(stb_image stb_image.cpp stb_image.h)
target_include_directories(stb_image PUBLIC)