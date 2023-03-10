cmake_minimum_required(VERSION 3.16)
project(bash_cli)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra-semi -g -fsanitize=address -fno-omit-frame-pointer")

FIND_PACKAGE(Boost 1.71 REQUIRED COMPONENTS system filesystem)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

include_directories(include)
include_directories(commands)
include_directories(src)


add_executable(bash_cli
        src/main.cpp
        src/application.cpp
        src/handler.cpp)



TARGET_LINK_LIBRARIES(bash_cli LINK_PUBLIC ${Boost_LIBRARIES})
