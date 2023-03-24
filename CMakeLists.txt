cmake_minimum_required(VERSION 3.16)
project(bash_cli)

set(CMAKE_CXX_STANDARD 20)



if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(MSVC)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
#        add_compile_options(/WX)
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX /O2")
#        add_compile_options(/WX /02)
    endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
else()
    find_package(Boost 1.71 COMPONENTS system filesystem)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -fsanitize=address -fno-omit-frame-pointer -v -pthread")
#        add_compile_options(-Wall -g -fsanitize=address -fno-omit-frame-pointer -v)
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -pthread")
#        add_compile_options(-Wall -O2)
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

add_executable(test_commands
        tests/test_commands.cpp)

gtest_discover_tests(test_commands)

if (Boost_FOUND)
    target_link_libraries(bash_cli LINK_PUBLIC ${Boost_LIBRARIES})
    target_link_libraries(test_analyzer LINK_PUBLIC ${Boost_LIBRARIES})
    target_link_libraries(test_commands LINK_PUBLIC GTest::gtest_main ${Boost_LIBRARIES} )
else()
    target_link_libraries(bash_cli PRIVATE Boost::process)
    target_link_libraries(test_analyzer PRIVATE Boost::process)
    target_link_libraries(test_commands PRIVATE GTest::gtest_main Boost::process)
endif()
