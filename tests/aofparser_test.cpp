#include <catch2/catch.hpp>

#include "simplekvdb/AOFParser.hpp"

using namespace simplekvdb::aoflogging;

TEST_CASE("Test parse set command") {
    std::string line {"SET|4|abcd|5|12345"};
    auto res = std::get<simplekvdb::SetCommand>(AOFParser::parseLine(line));
    REQUIRE (res.key == "abcd");
    REQUIRE (res.value == "12345");
}

TEST_CASE("Test parse del command") {
    std::string line {"DEL|8|12345678"};
    auto res = std::get<simplekvdb::DelCommand>(AOFParser::parseLine(line));
    REQUIRE (res.key == "12345678");
}