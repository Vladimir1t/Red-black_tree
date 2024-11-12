cmake_minimum_required(VERSION 3.11)
project("RB_tree")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(tree_srcs
    main.cpp
)
set(test_srcs
    tests/src/tests.cpp
)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(FLAGS   "-D NDEBUG")

add_executable(rb_tree.x ${tree_srcs})
add_executable(test.x    ${test_srcs})

target_include_directories(rb_tree.x PRIVATE 
            ${CMAKE_SOURCE_DIR}/include     
)
target_include_directories(test.x PRIVATE 
            ${CMAKE_SOURCE_DIR}/include        
)

target_compile_options(rb_tree.x  PRIVATE "$<$<CONFIG:RELEASE>:${CMAKE_CXX_FLAGS_RELEASE}>" ${FLAGS})
target_compile_options(test.x  PRIVATE "$<$<CONFIG:RELEASE>:${CMAKE_CXX_FLAGS_RELEASE}>" ${FLAGS})

# cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
# cmake --build build
# cmake -DCMAKE_BUILD_TYPE=Release
# ./build/rb_tree.x
#
# cmake .. -DCMAKE_CXX_INCLUDE_WHAT_YOU_USE=./../../../../include-what-you-use/build/bin/include-what-you-use
# make
