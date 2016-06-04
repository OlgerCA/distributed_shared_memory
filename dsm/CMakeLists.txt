
find_package(GLIB REQUIRED)
include_directories(${GLIB_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/dsm_common)

set(SOURCE_FILES main.c ServerHandle.c ServerHandle.h ServerForward.c ServerForward.h ServerPageEntry.h ClientEntry.h ServerPageEntry.c ClientEntry.c Logger.c Logger.h FileReader.c FileReader.h Server.c Server.h NetworkUtils.c NetworkUtils.h)
set(LIBS ${LIBS} ${GLIB_LIBRARIES})

add_executable(dsm ${SOURCE_FILES})
target_link_libraries(dsm ${LIBS} m)