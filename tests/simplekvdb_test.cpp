#include <catch2/catch.hpp>
#include "Database.hpp"

using namespace simplekvdb;

TEST_CASE("Example test case") {
    REQUIRE(1 + 1 == 2);
}

TEST_CASE("Database PUT, GET") {
    Database db;
    db.Put("abc", "cde");
    auto res = db.Get("abc").value();
    REQUIRE( res == "cde");
}

TEST_CASE("Database GET no exist") {
    Database db;
    auto res = db.Get("abc");
    REQUIRE (res == std::nullopt);
}

TEST_CASE("Database PUT exist already returns true") {
    Database db;
    auto res = db.Put("abc", "def");
    REQUIRE (res == false);
    auto res2 = db.Put("abc", "eds");
    REQUIRE (res2 == true);
}