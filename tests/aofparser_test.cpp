#include <catch2/catch.hpp>

#include "AOFParser.hpp"

using namespace simplekvdb::aoflogging;

TEST_CASE("Test parse set command") {
    std::string line {"SET|4|abcd|5|12345"};
    auto res = std::get<simplekvdb::SetCommand>(AOFParser::parseLine(line));
    REQUIRE (res.key == "abcd");
    REQUIRE (res.value == "12345");
}