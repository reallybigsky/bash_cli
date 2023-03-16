cmake_minimum_required(VERSION 3.16)
project(bash_cli)

set(CMAKE_CXX_STANDARD 20)



if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(MSVC)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX /O3")
    endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
else()
    find_package(Boost 1.74 REQUIRED COMPONENTS system filesystem)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -fsanitize=address -fno-omit-frame-pointer -v")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3")
    endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
endif()


if (Boost_FOUND)
    message("Boost found!")
    include_directories( ${Boost_INCLUDE_DIR} )
else()
    message("Boost not found, downloading...")
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
endif()

include_directories(include)
include_directories(commands)
include_directories(src)

add_executable(bash_cli
        src/main.cpp
        src/application.cpp
        src/handler.cpp)

add_executable(test_analyzer
        tests/test_analyzer.cpp)


if (Boost_FOUND)
    target_link_libraries(bash_cli LINK_PUBLIC ${Boost_LIBRARIES})
else()
    target_link_libraries(bash_cli PRIVATE Boost::process)
    target_link_libraries(test_analyzer PRIVATE Boost::process)
endif()