#include <catch2/catch.hpp>

#include "LoggingUtil.hpp"

using namespace simplekvdb;

TEST_CASE("Test setCommand") {
    auto res = aoflogging::stringifySetCommand("123", "abc");
    REQUIRE(res == "SET|3|123|3|abc");
}

TEST_CASE("Test delCommand") {
    auto res = aoflogging::stringifyDelCommand("abc");
    REQUIRE(res == "DEL|3|abc");
}

TEST_CASE("Test getFileName") {
    auto res = aoflogging::getFileName(1);
    REQUIRE(res == "simplekvdb_1.store");
}