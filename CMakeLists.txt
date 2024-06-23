cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(byrep)

file(GLOB byrep_sources src/*.cpp)
add_executable(byrep ${byrep_sources})
target_compile_features(byrep PRIVATE cxx_std_17)

install(TARGETS byrep)
