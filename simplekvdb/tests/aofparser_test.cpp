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
    REQUIRE (res.keys[0] == "12345678");
}

TEST_CASE("Test parse del command multiple keys") {
    std::string line {"DEL|11|12345678901|2|ab|4|1234"};
    auto res = std::get<simplekvdb::DelCommand>(AOFParser::parseLine(line));
    REQUIRE (res.keys[0] == "12345678901");
    REQUIRE (res.keys[1] == "ab");
    REQUIRE (res.keys[2] == "1234");
}

TEST_CASE("Test parse hdel command") {
    std::string line {"HDEL|4|abcd|2|ab|3|123"};
    auto res = std::get<simplekvdb::HDelCommand>(AOFParser::parseLine(line));
    REQUIRE (res.key == "abcd");
    REQUIRE (res.fields[0] == "ab");
    REQUIRE (res.fields[1] == "123");
}

TEST_CASE("Test parse hset command") {
    std::string line {"HSET|4|abcd|2|ab|3|123|10|1234567890|1|1"};
    auto res = std::get<simplekvdb::HSetCommand>(AOFParser::parseLine(line));
    REQUIRE (res.key == "abcd");
    REQUIRE (res.fieldValuePairs[0].first == "ab");
    REQUIRE (res.fieldValuePairs[0].second == "123");
    REQUIRE (res.fieldValuePairs[1].first == "1234567890");
    REQUIRE (res.fieldValuePairs[1].second == "1");
}