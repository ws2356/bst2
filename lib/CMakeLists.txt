cmake_minimum_required(VERSION 3.0.0)

add_library(bst INTERFACE)
target_compile_features(bst INTERFACE cxx_auto_type)
target_include_directories(bst INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})