enable_testing()

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.cpp)
set(SOURCE_FILES ${TEST_SOURCES})

add_executable(tests ${SOURCE_FILES})
target_link_libraries(tests PRIVATE simplekvdb)

# Include the Catch2 header file
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/external/include)

# Discover and register tests
add_test(NAME catch_tests COMMAND tests)
