
include_directories(${CMAKE_SOURCE_DIR}/dsm_common)
set(SOURCE_FILES DSMClient.c DSMClient.h ClientRequest.c ClientRequest.h ClientHandle.c ClientHandle.h ClientPageEntry.h)
add_library(dsm_library SHARED ${SOURCE_FILES})