cmake_minimum_required(VERSION 3.14)
project(s_coll.test)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(s_coll.test
    src/p_array_test.cpp
)
target_link_libraries(s_coll.test
    GTest::gtest_main
)
target_include_directories(s_coll.test
    PUBLIC include
    PUBLIC ../include
)

include(GoogleTest)
gtest_discover_tests(s_coll.test)
