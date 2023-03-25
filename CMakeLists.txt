cmake_minimum_required(VERSION 3.16)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()


project(bash_cli)

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(MSVC)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX /O2")
    endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
else()
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -fsanitize=address -fno-omit-frame-pointer -v")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")
    endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
endif()

set(BOOST_INCLUDE_LIBRARIES process program_options)
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

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
IF (WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
ENDIF()
FetchContent_MakeAvailable(googletest)
enable_testing()
include(GoogleTest)

include_directories(include)
include_directories(commands)
include_directories(src)

add_executable(bash_cli
        src/main.cpp
        src/application.cpp
        src/handler.cpp)

add_executable(test_analyzer
        tests/test_analyzer.cpp)

if(MINGW)
    target_link_libraries(bash_cli PRIVATE Boost::process ws2_32)
    target_link_libraries(test_analyzer PRIVATE Boost::process ws2_32)
else()
    target_link_libraries(bash_cli PRIVATE Boost::process Boost::program_options)
    target_link_libraries(test_analyzer PRIVATE Boost::process)
endif()
