cmake_minimum_required(VERSION 3.17)
project(bash_cli)

set(CMAKE_CXX_STANDARD 20)

FIND_PACKAGE(Boost 1.76 REQUIRED COMPONENTS system filesystem)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

include_directories(include)
include_directories(commands)
include_directories(src)

add_executable(bash_cli
        src/main.cpp
        src/application.cpp
        src/handler.cpp
        )

TARGET_LINK_LIBRARIES(bash_cli LINK_PUBLIC ${Boost_LIBRARIES})