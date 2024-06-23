cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(byrep LANGUAGES CXX)

option(BYREP_BUILD_CLI "Build the Byrep CLI tool" ON)

file(GLOB lib_sources lib/*.cpp)
add_library(byrep ${lib_sources})
target_compile_features(byrep PUBLIC cxx_std_17)
target_include_directories(byrep PUBLIC include)

if(BYREP_BUILD_CLI)
  file(GLOB cli_sources src/*.cpp)
  add_executable(byrep_cli ${cli_sources})
  target_link_libraries(byrep_cli PRIVATE byrep)
  set_target_properties(byrep_cli PROPERTIES OUTPUT_NAME byrep)

  install(TARGETS byrep_cli)
endif()

install(TARGETS byrep)
install(DIRECTORY include/byrep DESTINATION include)
