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
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX /O3")
    endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
else()
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra -Wunused-parameter \
        -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wduplicated-branches \
        -Wduplicated-cond -Wextra-semi -Wfloat-equal -Wlogical-op \
        -Wnon-virtual-dtor -Woverloaded-virtual -Wredundant-decls -Wsign-promo \
        -pedantic-errors \
        -g -fsanitize=address -fno-omit-frame-pointer")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra -Wunused-parameter \
        -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wduplicated-branches \
        -Wduplicated-cond -Wextra-semi -Wfloat-equal -Wlogical-op \
        -Wnon-virtual-dtor -Woverloaded-virtual -Wredundant-decls -Wsign-promo \
        -pedantic-errors \
        -O3")
    endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
endif()

find_package(Boost 1.74 COMPONENTS system filesystem program_options)
if (Boost_FOUND)
    message("Boost found!")
    include_directories( ${Boost_INCLUDE_DIR} )
else()
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
endif()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)
enable_testing()
include(GoogleTest)

include_directories(include)
include_directories(commands)
include_directories(src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(bash_cli
        src/main.cpp
        src/application.cpp
        src/handler.cpp)

# tests output dir!
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../tests)

add_executable(test_analyzer
        tests/src/test_analyzer.cpp)

add_executable(test_commands
        tests/src/test_commands.cpp)

add_executable(test_grep
        tests/src/test_grep.cpp)

add_executable(test_runner
        tests/src/test_runner.cpp
        src/application.cpp
        src/handler.cpp)

if(MINGW)
    target_link_libraries(bash_cli PRIVATE Boost::process Boost::program_options ws2_32)
    target_link_libraries(test_analyzer PRIVATE GTest::gtest_main Boost::process Boost::program_options ws2_32)
    target_link_libraries(test_commands PRIVATE GTest::gtest_main Boost::process Boost::program_options ws2_32)
    target_link_libraries(test_grep PRIVATE GTest::gtest_main Boost::process Boost::program_options ws2_32)
    target_link_libraries(test_runner PRIVATE GTest::gtest_main Boost::process Boost::program_options ws2_32)
else()
    if (Boost_FOUND)
        target_link_libraries(bash_cli PRIVATE ${Boost_LIBRARIES})
        target_link_libraries(test_analyzer PRIVATE ${Boost_LIBRARIES} GTest::gtest_main)
        target_link_libraries(test_commands PRIVATE ${Boost_LIBRARIES} GTest::gtest_main)
        target_link_libraries(test_grep PRIVATE ${Boost_LIBRARIES} GTest::gtest_main)
        target_link_libraries(test_runner PRIVATE ${Boost_LIBRARIES} GTest::gtest_main)
    else()
        target_link_libraries(bash_cli PRIVATE Boost::process Boost::program_options)
        target_link_libraries(test_analyzer PRIVATE GTest::gtest_main Boost::process Boost::program_options)
        target_link_libraries(test_commands PRIVATE GTest::gtest_main Boost::process Boost::program_options)
        target_link_libraries(test_grep PRIVATE GTest::gtest_main Boost::process Boost::program_options)
        target_link_libraries(test_runner PRIVATE GTest::gtest_main Boost::process Boost::program_options)
    endif()
endif()
