cmake_minimum_required(VERSION 3.18)

project(Shmurp VERSION "${BUILD_VERSION}")

include(cmake/include.cmake)

cmc_include_conan_configuration()

add_subdirectory(src)
