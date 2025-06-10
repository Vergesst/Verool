cmake_minimum_required(VERSION 3.29)
project(mem_pool)

set(CMAKE_CXX_STANDARD 20)

include_directories(./mem_pool 
                    ./mem_pool/allocator
                    )
                    
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")

add_executable(mem_pool main.cpp)
# test malloc executable
add_executable(test_malloc test_malloc/main.cpp)
add_executable(test_array test_array/main.cpp)
# add_executable(test test.cpp)

# test
add_test(NAME make_test COMMAND test_running)