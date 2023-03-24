#include <catch2/catch.hpp>
#include "main.hpp"

TEST_CASE("Example test case") {
    REQUIRE(1 + 1 == 2);
}


TEST_CASE("Example test") {
    Test2 test2;
    REQUIRE(test2.return42() == 42);
}
