
cmake_minimum_required(VERSION 3.3)
project(distributed_shared_memory)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

add_subdirectory(application_1)
add_subdirectory(application_2)
add_subdirectory(application_3)
add_subdirectory(dsm)
add_subdirectory(dsm_library)
add_subdirectory(dsm_common)