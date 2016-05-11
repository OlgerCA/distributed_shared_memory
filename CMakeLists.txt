
cmake_minimum_required(VERSION 3.3)
project(distributed_shared_memory)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

add_subdirectory(application_1)
add_subdirectory(application_2)
add_subdirectory(application_3)
add_subdirectory(dsm)
add_subdirectory(dsm_library)