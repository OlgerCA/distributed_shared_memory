
include_directories(${CMAKE_SOURCE_DIR}/dsm_library)
set(SOURCE_FILES main.c)
add_executable(application_3 ${SOURCE_FILES})
target_link_libraries(application_3 dsm_library ${LIBS})