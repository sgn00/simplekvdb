#include <catch2/catch.hpp>

#include "simplekvdb/AOFLoader.hpp"
#include "simplekvdb/KvStore.hpp"
#include "testutil.hpp"

using namespace simplekvdb;

TEST_CASE("Log, Load and Parse Test 1") {
  DeleteFile df(1);
  {
    KvStore kvStore(1, 100, true);
    for (int i = 0; i < 10; i++) {
      kvStore.set(std::to_string(i), std::to_string(i));
    }
    for (int i = 0; i < 10; i++) {
      kvStore.get(std::to_string(i));
    }
    for (int i = 0; i < 5; i++) {
      kvStore.del({std::to_string(i)});
    }
  }

  int numExpected = 10 - 5;

  KvStore kvStore(1, 100, true);
  REQUIRE(kvStore.size() == numExpected);
  for (int i = 6; i < 10; i++) {
    auto res = kvStore.get(std::to_string(i));
    REQUIRE(res.status() == Result::Status::OK);
  }
}

TEST_CASE("Log, Load and Parse Test 2 for H Commands") {
  DeleteFile df(2);
  {
    KvStore kvStore(2, 100, true);
    for (int i = 0; i < 10; i++) {
      kvStore.hset(std::to_string(i), {{std::to_string(i), "a"}, {"b", "cde"}});
    }
    for (int i = 0; i < 10; i++) {
      kvStore.hget(std::to_string(i), "b");
    }
    for (int i = 0; i < 5; i++) {
      kvStore.hdel(std::to_string(i), {"b"});
    }
  }

  int numExpected = 10;

  KvStore kvStore(2, 100, true);
  REQUIRE(kvStore.size() == 10);
  for (int i = 0; i < 5; i++) {
    auto res = kvStore.hget(std::to_string(i), "b");
    REQUIRE(res.status() == Result::Status::Error);
    REQUIRE(res.error_code() == Result::ErrorCode::FieldNotFound);
  }
  for (int i = 6; i < 10; i++) {
    auto res = kvStore.hget(std::to_string(i), "b");
    REQUIRE(res.status() == Result::Status::OK);
    REQUIRE(res.value<std::string>() == "cde");
  }
}