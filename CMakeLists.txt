cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(EXPORT_COMPILE_COMMANDS ON)

project(
  fish-school
  VERSION 0.0.1
  DESCRIPTION "Simulator of fish schooling"
  LANGUAGES CXX C)

include(ProjectOptions.cmake)
project_setup_options()

add_subdirectory(src)