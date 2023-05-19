cmake_minimum_required(VERSION 3.20)
project(LFA)
SET(CMAKE_CXX_STANDARD 20)
option(SANITIZE "Enable sanitizers" OFF)
if(SANITIZE)
    message(STATUS "Sanitizers enabled")
    if(WIN32)
        message(WARNING "Sanitizers may fail to link on Windows")
    endif()
    set(CMAKE_CXX_FLAGS
        "-Wall -Wextra -pedantic -fsanitize=address,leak,undefined -fno-omit-frame-pointer -g"
    )
endif()
add_executable(LFA "main.cpp" L_NFA.cpp L_NFA.h interactive_testing.cpp interactive_testing.h file_testing.h file_testing.cpp)
add_custom_command(
        TARGET LFA POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/input.txt
        ${CMAKE_CURRENT_BINARY_DIR}/input.txt)
