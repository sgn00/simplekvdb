enable_testing()

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.cpp)
set(SOURCE_FILES ${TEST_SOURCES})

add_executable(simplekvdb_tests ${SOURCE_FILES})
target_link_libraries(simplekvdb_tests PRIVATE simplekvdb)

find_package(Threads REQUIRED)
target_link_libraries(simplekvdb_tests PRIVATE Threads::Threads)

# Include the Catch2 header file
target_include_directories(simplekvdb_tests PRIVATE ${CMAKE_SOURCE_DIR}/external/include)

# Discover and register tests
add_test(NAME simplekvdb_tests COMMAND simplekvdb_tests)
