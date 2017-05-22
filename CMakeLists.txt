cmake_minimum_required(VERSION 3.7)

PROJECT(HttpServer)

set(CMAKE_C_FLAGS "-std=c99 -Wall -g")
set(CMAKE_CXX_FLAGS "-Wall -g")



# source files
file(GLOB_RECURSE SRCS2 src/Client.cpp src/Client.h)
file(GLOB_RECURSE SRCS3 src/Server.cpp src/Server.h)
file(GLOB_RECURSE SRCS4 src/serverRunner.cpp)
file(GLOB_RECURSE SRCS5 src/clientRunner.cpp)
file(GLOB_RECURSE SRCS6 src/HttpBuilder.cpp src/HttpBuilder.h)

# libraries and executables
add_library(Client ${SRCS2})
add_library(Server ${SRCS3})
add_library(HttpBuilder ${SRCS6})
add_executable(serverRunner ${SRCS4} .)
add_executable(clientRunner ${SRCS5} .)

# linking libraries to executables
target_link_libraries(serverRunner Server)
target_link_libraries(clientRunner Client)
target_link_libraries(Client HttpBuilder)
target_link_libraries(Server HttpBuilder)

# linking language setting
set_target_properties(Client PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(Server PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(serverRunner PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(clientRunner PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(HttpBuilder PROPERTIES LINKER_LANGUAGE CXX)

# setting output folder
set_target_properties(Client PROPERTIES RUNTIME_OUTPUT_DIRECTORY out)
set_target_properties(Server PROPERTIES RUNTIME_OUTPUT_DIRECTORY out)
set_target_properties(serverRunner PROPERTIES RUNTIME_OUTPUT_DIRECTORY out)
set_target_properties(clientRunner PROPERTIES RUNTIME_OUTPUT_DIRECTORY out)
set_target_properties(HttpBuilder PROPERTIES RUNTIME_OUTPUT_DIRECTORY out)
