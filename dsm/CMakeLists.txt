
include_directories(${CMAKE_SOURCE_DIR}/dsm_common)
set(SOURCE_FILES main.c ServerHandle.c ServerHandle.h ServerForward.c ServerForward.h ServerPageEntry.h)
add_executable(dsm ${SOURCE_FILES})
target_link_libraries(dsm ${LIBS})