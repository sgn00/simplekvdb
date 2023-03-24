file(GLOB CLI_APP_SOURCES CONFIGURE_DEPENDS "src/*.cpp" "include/*.hpp")

add_executable(cli_app ${CLI_APP_SOURCES})
target_include_directories(cli_app PRIVATE include)
target_link_libraries(cli_app PRIVATE simplekvdb)
