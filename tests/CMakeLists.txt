cmake_minimum_required(VERSION 3.17)
project(test)

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24:
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()


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



set(CMAKE_CXX_STANDARD 20)
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
IF (WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
ENDIF()

FetchContent_MakeAvailable(googletest)

include_directories(../include)
include_directories(../commands)
include_directories(../src)

enable_testing()

add_executable(
        test_commands
        test_commands.cpp
)

target_link_libraries(
        test_commands
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_commands)