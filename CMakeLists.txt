cmake_minimum_required(VERSION 3.16)
project(bash_cli)

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -fsanitize=address -fno-omit-frame-pointer -v")
endif()

#FIND_PACKAGE(Boost 1.76 REQUIRED COMPONENTS system filesystem)
#INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

set(BOOST_INCLUDE_LIBRARIES process)
set(BOOST_ENABLE_CMAKE ON)
set(FETCHCONTENT_QUIET FALSE)

include(FetchContent)
FetchContent_Declare(
        Boost
        GIT_REPOSITORY https://github.com/boostorg/boost.git
        GIT_TAG boost-1.81.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(Boost)

include_directories(include)
include_directories(commands)
include_directories(src)

add_executable(bash_cli
        src/main.cpp
        src/application.cpp
        src/handler.cpp)

add_executable(test_analyzer
        tests/test_analyzer.cpp)

target_link_libraries(bash_cli PRIVATE Boost::process)
target_link_libraries(test_analyzer PRIVATE Boost::process)
#TARGET_LINK_LIBRARIES(bash_cli LINK_PUBLIC ${Boost_LIBRARIES})

