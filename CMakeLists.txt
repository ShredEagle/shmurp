cmake_minimum_required(VERSION 3.14)

project(Shmurp)

include(cmake/include.cmake)

add_compile_definitions(BOOST_AUTO_LINK_SYSTEM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)
