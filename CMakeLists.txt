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

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

option(SET_MODE_ENABLED "Enable SET mode" OFF)

set(CUSTOM_SET_VALUE "" CACHE STRING "Режим исполнения")

if(SET_MODE_ENABLED)
    message(STATUS "SET mode enabled")
    add_definitions(-DSET_MODE_ENABLED)  
else()
    message(STATUS "SET mode not enabled")
endif()

if(CUSTOM_SET_VALUE STREQUAL "DEBUG")
    message(STATUS "Custom mode: DEBUG")
    add_compile_definitions(CUSTOM_MODE_DEBUG)
elseif(CUSTOM_SET_VALUE STREQUAL "RELEASE")
    message(STATUS "Custom mode: RELEASE")
    add_compile_definitions(CUSTOM_MODE_RELEASE)
else()
    message(STATUS "Custom mode: Default")
endif()


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
target_link_libraries(test.x GTest::GTest GTest::Main)

target_compile_options(rb_tree.x  PRIVATE "$<$<CONFIG:RELEASE>:${CMAKE_CXX_FLAGS_RELEASE}>" ${FLAGS})
target_compile_options(test.x  PRIVATE "$<$<CONFIG:RELEASE>:${CMAKE_CXX_FLAGS_RELEASE}>" ${FLAGS})

# cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
# cmake -DSET_MODE_ENABLED=ON -DCUSTOM_SET_VALUE=DEBUG -S . -B build
# cmake --build build
# cmake -DCMAKE_BUILD_TYPE=Release
# ./build/rb_tree.x
#
# cmake .. -DCMAKE_CXX_INCLUDE_WHAT_YOU_USE=./../../../../include-what-you-use/build/bin/include-what-you-use
# make
