project(LFA)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
SET(CMAKE_CXX_STANDARD 20)
add_executable(LFA "main.cpp" L_NFA.cpp L_NFA.h interactive_testing.cpp interactive_testing.h)
