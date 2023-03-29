file(GLOB kvclient_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(kvclient ${kvclient_SOURCES})
find_package(Threads REQUIRED)
target_include_directories(kvclient PRIVATE include)
target_link_libraries(kvclient PRIVATE simplekvdb rpc Threads::Threads)
