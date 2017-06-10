cmake_minimum_required(VERSION 3.7)

PROJECT(HttpServer)
set(CMAKE_C_FLAGS "-Wall -g")
set(CMAKE_CXX_FLAGS "-g -std=c++11 -Wall")
set(LDFLAGS "-g")
set(CMAKE_BUILD_TYPE Debug)



# source files
file(GLOB_RECURSE SRCS3 src/Server.cpp src/Server.h)
file(GLOB_RECURSE SRCS4 src/serverRunner.cpp)
file(GLOB_RECURSE SRCS6 src/HttpBuilder.cpp src/HttpBuilder.h)
file(GLOB_RECURSE SRCS7 src/FileMenager.cpp src/FileMenager.h)
file(GLOB_RECURSE SRCS8 src/HttpParser.cpp src/HttpParser.h)
file(GLOB_RECURSE SRCS9 src/ClientQueue.cpp src/ClientQueue.h)
file(GLOB_RECURSE SRCS10 src/Configurator.cpp src/Configurator.h)
file(GLOB_RECURSE SRCS11 src/Logger.cpp src/Logger.h)
file(GLOB_RECURSE SRCS12 src/exampleScript.cpp)

# libraries and executables
add_library(Server ${SRCS3})
add_library(HttpBuilder ${SRCS6})
add_library(FileMenager ${SRCS7})
add_library(HttpParser ${SRCS8})
add_library(ClientQueue ${SRCS9})
add_library(Configurator ${SRCS10})
add_library(Logger ${SRCS11})
add_executable(serverRunner ${SRCS4} .)
add_executable(exampleScript.cgi ${SRCS12} .)

# linking libraries to executables
target_link_libraries(serverRunner Server)
target_link_libraries(serverRunner Configurator)
target_link_libraries(Server HttpBuilder)
target_link_libraries(Server Logger)
target_link_libraries(Server FileMenager)
target_link_libraries(Server HttpParser)
target_link_libraries(Server ClientQueue)

# linking language setting
set_target_properties(Server PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(serverRunner PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(HttpBuilder PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(FileMenager PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(HttpParser PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ClientQueue PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(Configurator PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(Logger PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(exampleScript.cgi PROPERTIES LINKER_LANGUAGE CXX)

# setting output folder
set_target_properties(Server PROPERTIES RUNTIME_OUTPUT_DIRECTORY out)
set_target_properties(serverRunner PROPERTIES RUNTIME_OUTPUT_DIRECTORY out)
set_target_properties(HttpBuilder PROPERTIES RUNTIME_OUTPUT_DIRECTORY out)
set_target_properties(FileMenager PROPERTIES RUNTIME_OUTPUT_DIRECTORY out)
set_target_properties(HttpParser PROPERTIES RUNTIME_OUTPUT_DIRECTORY out)
set_target_properties(ClientQueue PROPERTIES RUNTIME_OUTPUT_DIRECTORY out)
set_target_properties(Configurator PROPERTIES RUNTIME_OUTPUT_DIRECTORY out)
set_target_properties(Configurator PROPERTIES RUNTIME_OUTPUT_DIRECTORY out)
set_target_properties(exampleScript.cgi PROPERTIES RUNTIME_OUTPUT_DIRECTORY out)
