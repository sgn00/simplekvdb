file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS "src/*.cpp" "include/*.hpp")

add_library(simplekvdb STATIC ${LIB_SOURCES})
target_include_directories(simplekvdb PUBLIC include)
