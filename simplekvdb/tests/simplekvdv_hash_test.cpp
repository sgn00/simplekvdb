#include <catch2/catch.hpp>

#include "simplekvdb/KvStore.hpp"
#include "testutil.hpp"

using namespace simplekvdb;

TEST_CASE("HSET basic success test") {
  KvStore kvStore(20, 100, false);
  auto res = kvStore.hset("hhh", {{"abc", "123"}, {"ghi", "456"}});
  REQUIRE(res.status() == Result::Status::OK);
  REQUIRE(res.value<int>() == 2);
}

TEST_CASE("HSET, HGET basic success test") {
  KvStore kvStore(20, 100, false);
  kvStore.hset("hhh", {{"abc", "123"}, {"ghi", "456"}});
  auto res = kvStore.hget("hhh", "abc");
  REQUIRE(res.status() == Result::Status::OK);
  REQUIRE(res.value<std::string>() == "123");

  auto res2 = kvStore.hget("hhh", "ghi");
  REQUIRE(res2.status() == Result::Status::OK);
  REQUIRE(res2.value<std::string>() == "456");
}

TEST_CASE("HSET, HDEL basic success test") {
  KvStore kvStore(20, 100, false);
  kvStore.hset("hhh", {{"abc", "123"}, {"ghi", "456"}});
  auto res = kvStore.hdel("hhh", {"abc"});
  REQUIRE(res.status() == Result::Status::OK);
  REQUIRE(res.value<int>() == 1);
}

TEST_CASE("HSET, HDEL, HGET basic test") {
  KvStore kvStore(20, 100, false);
  kvStore.hset("hhh", {{"abc", "123"}, {"ghi", "456"}});
  kvStore.hdel("hhh", {"abc"});
  auto res = kvStore.hget("hhh", "abc");
  REQUIRE(res.status() == Result::Status::Error);
  REQUIRE(res.error_code() == Result::ErrorCode::FieldNotFound);
  auto res2 = kvStore.hget("hhh", "ghi");
  REQUIRE(res2.status() == Result::Status::OK);
  REQUIRE(res2.value<std::string>() == "456");
}

TEST_CASE("HDEL multiple test") {
  KvStore kvStore(20, 100, false);
  kvStore.hset("hhh", {{"abc", "123"}, {"ghi", "456"}});
  auto res = kvStore.hdel("hhh", {"abc", "ghi"});
  REQUIRE(res.status() == Result::Status::OK);
  REQUIRE(res.value<int>() == 2);
  auto res2 = kvStore.hget("hhh", "ghi");
  REQUIRE(res2.status() == Result::Status::Error);
  REQUIRE(res2.error_code() == Result::ErrorCode::FieldNotFound);
  auto res3 = kvStore.hget("hhh", "abc");
  REQUIRE(res2.status() == Result::Status::Error);
  REQUIRE(res2.error_code() == Result::ErrorCode::FieldNotFound);
}

TEST_CASE("HGET on string key wrong type") {
  KvStore kvStore(20, 100, false);
  kvStore.set("abc", "def");
  auto res = kvStore.hget("abc", "def");
  REQUIRE(res.status() == Result::Status::Error);
  REQUIRE(res.error_code() == Result::ErrorCode::WrongType);
}

TEST_CASE("HSET continuous, fields are cumulative") {
  KvStore kvStore(20, 100, false);
  kvStore.hset("hhh", {{"abc", "123"}});
  kvStore.hset("hhh", {{"cde", "456"}});
  auto res = kvStore.hget("hhh", "abc");
  REQUIRE(res.status() == Result::Status::OK);
  REQUIRE(res.value<std::string>() == "123");
  auto res2 = kvStore.hget("hhh", "cde");
  REQUIRE(res2.status() == Result::Status::OK);
  REQUIRE(res2.value<std::string>() == "456");
}
