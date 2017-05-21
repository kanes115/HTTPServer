cmake_minimum_required(VERSION 3.7)

PROJECT(HttpServer)

set(CMAKE_C_FLAGS "-std=c99 -Wall -g")
set(CMAKE_CXX_FLAGS "-Wall -g")


file(GLOB SRCS2 Client.cpp Client.h)
file(GLOB SRCS3 Server.cpp Server.h)
file(GLOB SRCS4 serverRunner.cpp)
file(GLOB SRCS5 clientRunner.cpp)

add_library(Client ${SRCS2})
add_library(Server ${SRCS3})
#add_executable(Client ${SRCS2})
#add_executable(Server ${SRCS3})
add_executable(serverRunner ${SRCS4} .)
add_executable(clientRunner ${SRCS5} .)

#target_link_libraries(main Client)
target_link_libraries(serverRunner Server)
target_link_libraries(clientRunner Client)


set_target_properties(Client PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(Server PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(serverRunner PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(clientRunner PROPERTIES LINKER_LANGUAGE CXX)

set_target_properties(Client PROPERTIES RUNTIME_OUTPUT_DIRECTORY out)
set_target_properties(Server PROPERTIES RUNTIME_OUTPUT_DIRECTORY out)
set_target_properties(serverRunner PROPERTIES RUNTIME_OUTPUT_DIRECTORY out)
set_target_properties(clientRunner PROPERTIES RUNTIME_OUTPUT_DIRECTORY out)
