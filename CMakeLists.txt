cmake_minimum_required(VERSION 3.0)
project(LFA)
SET(CMAKE_CXX_STANDARD 20)
add_executable(LFA "main.cpp" L_NFA.cpp L_NFA.h interactive_testing.cpp interactive_testing.h file_testing.h file_testing.cpp)
add_custom_command(
        TARGET LFA POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/input.txt
        ${CMAKE_CURRENT_BINARY_DIR}/input.txt)
