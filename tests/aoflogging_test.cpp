#include <catch2/catch.hpp>

#include "LoggingUtil.hpp"

using namespace simplekvdb;

TEST_CASE("Test setCommand") {
    auto res = aoflogging::setCommand("123", "abc");
    REQUIRE(res == "GET|3|123|3|abc");
}

TEST_CASE("Test delCommand") {
    auto res = aoflogging::delCommand("abc");
    REQUIRE(res == "DEL|abc");
}

TEST_CASE("Test getFileName") {
    auto res = aoflogging::getFileName(1);
    REQUIRE(res == "simplekvdb_1.store");
}