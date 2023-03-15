cmake_minimum_required(VERSION 3.17)
project(test)

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24:
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
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