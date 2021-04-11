cmake_minimum_required(VERSION 3.7)
project(sdml)
set(CMAKE_CXX_STANDARD 20)


set(SDML_CMAKE_MODULE_PATH  ${CMAKE_SOURCE_DIR}/build-util/cmake-module)
set(SDML_INC 
${CMAKE_SOURCE_DIR}/src/include
)

find_package(Boost REQUIRED)
find_package(fmt REQUIRED)
find_package(utf8cpp REQUIRED)
find_package(re2 REQUIRED)

set(SDML_DEP_LIB
utf8cpp
fmt::fmt
Boost::boost 
re2::re2
)

execute_process(COMMAND python ${CMAKE_SOURCE_DIR}/build-util/build_prerun.py)
include(${SDML_CMAKE_MODULE_PATH}/sdml_var.cmake)


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

add_subdirectory(src)