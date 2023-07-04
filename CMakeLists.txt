cmake_minimum_required(VERSION 3.8)

project(toystl)

set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_STANDARD 11)

message(STATUS "start running cmake...")

set(SOURCE_FILES 
${PROJECT_SOURCE_DIR}/toy/main.cpp
${PROJECT_SOURCE_DIR}/toy/stl_algobase.h
${PROJECT_SOURCE_DIR}/toy/stl_allocator.h
${PROJECT_SOURCE_DIR}/toy/stl_config.h
${PROJECT_SOURCE_DIR}/toy/stl_construct.h
${PROJECT_SOURCE_DIR}/toy/stl_iterator_base.h
${PROJECT_SOURCE_DIR}/toy/stl_iterator.h
${PROJECT_SOURCE_DIR}/toy/stl_uninitialized.h
${PROJECT_SOURCE_DIR}/toy/stl_vector.h
${PROJECT_SOURCE_DIR}/toy/type_traits.h
${PROJECT_SOURCE_DIR}/toy/utlity.h
${PROJECT_SOURCE_DIR}/toy/stl_pair.h
)

add_executable(toystl ${SOURCE_FILES})