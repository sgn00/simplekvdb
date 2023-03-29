file(GLOB KVSERVER_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(kvserver ${KVSERVER_SOURCES})
target_include_directories(kvserver PRIVATE include)
target_link_libraries(kvserver PRIVATE simplekvdb rpc)
