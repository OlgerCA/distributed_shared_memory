
include_directories(${CMAKE_SOURCE_DIR}/dsm_library)
set(SOURCE_FILES main.c)
add_executable(application_1 ${SOURCE_FILES})
target_link_libraries(application_1 dsm_library ${LIBS})