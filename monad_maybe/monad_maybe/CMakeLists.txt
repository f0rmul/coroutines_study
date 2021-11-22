# CMakeList.txt : CMake project for monad_maybe, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

add_library(resumable INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/resumable.hpp)
set_target_properties(resumable PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(resumable PROPERTIES CXX_STANDARD 20)


add_executable (monad_maybe "main.cpp")

target_link_libraries(monad_maybe resumable)