cmake_minimum_required(VERSION 3.16)
project(test)

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
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -fsanitize=address -fno-omit-frame-pointer -v")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3")
    endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
endif()


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

if (Boost_FOUND)
    target_link_libraries(test LINK_PUBLIC ${Boost_LIBRARIES})
else()
    target_link_libraries(test PRIVATE Boost::process)
    target_link_libraries(test_analyzer PRIVATE Boost::process)
endif()


include(GoogleTest)
gtest_discover_tests(test_commands)

cmake_minimum_required(VERSION 3.17)
project(test)




