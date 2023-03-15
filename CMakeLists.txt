project(LFA)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

add_executable(LFA "main.cpp" L_NFA.cpp L_NFA.h)
