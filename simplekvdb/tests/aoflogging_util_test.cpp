#include <catch2/catch.hpp>

#include "simplekvdb/LoggingUtil.hpp"

using namespace simplekvdb;

TEST_CASE("Test setCommand") {
  auto res = aoflogging::stringifySetCommand("123", "abc");
  REQUIRE(res == "SET|3|123|3|abc");
}

TEST_CASE("Test delCommand") {
  auto res = aoflogging::stringifyDelCommand({"abc"});
  REQUIRE(res == "DEL|3|abc");
}

TEST_CASE("Test delCommand multiple keys") {
  auto res = aoflogging::stringifyDelCommand({"abc", "def", "gh"});
  REQUIRE(res == "DEL|3|abc|3|def|2|gh");
}

TEST_CASE("Test hsetCommand") {
  auto res =
      aoflogging::stringifyHsetCommand("abc", {{"123", "456"}, {"xyza", "1"}});
  REQUIRE(res == "HSET|3|abc|3|123|3|456|4|xyza|1|1");
}

TEST_CASE("Test hdelCommand") {
  auto res = aoflogging::stringifyHdelCommand("abc", {"123", "xyza"});
  REQUIRE(res == "HDEL|3|abc|3|123|4|xyza");
}

TEST_CASE("Test getFileName") {
  auto res = aoflogging::getFileName(1);
  REQUIRE(res == "simplekvdb_1.store");
}