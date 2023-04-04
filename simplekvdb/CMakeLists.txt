file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_library(simplekvdb STATIC ${LIB_SOURCES})

add_subdirectory(tests)

target_include_directories(simplekvdb PUBLIC 
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)