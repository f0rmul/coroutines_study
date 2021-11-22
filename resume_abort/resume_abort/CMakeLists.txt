# CMakeList.txt : CMake project for resume_abort, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_library(resumable INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/resumable.hpp)
set_target_properties(resumable PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(resumable PROPERTIES CXX_STANDARD 20)


add_executable (resume_abort "main.cpp")

target_link_libraries(resume_abort resumable)
# TODO: Add tests and install targets if needed.
