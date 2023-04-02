#include <catch2/catch.hpp>

#include "simplekvdb/KvStore.hpp"
#include "testutil.hpp"

using namespace simplekvdb;

TEST_CASE("HSET basic success test") {
    DeleteFile df(20);
    KvStore kvStore(20, 100, false);
    auto res = kvStore.hset("hhh", {{"abc", "123"}, {"ghi", "456"}});
    REQUIRE (res.status() == Result::Status::OK);
    REQUIRE (res.value<int>() == 2);
}

TEST_CASE("HSET, HGET basic success test") {
    DeleteFile df(20);
    KvStore kvStore(20, 100, false);
    kvStore.hset("hhh", {{"abc", "123"}, {"ghi", "456"}});
    auto res = kvStore.hget("hhh", "abc");
    REQUIRE (res.status() == Result::Status::OK);
    REQUIRE (res.value<std::string>() == "123");

    auto res2 = kvStore.hget("hhh", "ghi");
    REQUIRE (res2.status() == Result::Status::OK);
    REQUIRE (res2.value<std::string>() == "456");
}

TEST_CASE("HSET, HDEL basic success test") {
    DeleteFile df(20);
    KvStore kvStore(20, 100, false);
    kvStore.hset("hhh", {{"abc", "123"}, {"ghi", "456"}});
    auto res = kvStore.hdel("hhh", {"abc"});
    REQUIRE (res.status() == Result::Status::OK);
    REQUIRE (res.value<int>() == 1);
}

TEST_CASE("HSET, HDEL, HGET basic test") {
    DeleteFile df(20);
    KvStore kvStore(20, 100, false);
    kvStore.hset("hhh", {{"abc", "123"}, {"ghi", "456"}});
    kvStore.hdel("hhh", {"abc"});
    auto res = kvStore.hget("hhh", "abc");
    REQUIRE (res.status() == Result::Status::Error);
    REQUIRE (res.error_code() == Result::ErrorCode::FieldNotFound);
    auto res2 = kvStore.hget("hhh", "ghi");
    REQUIRE (res2.status() == Result::Status::OK);
    REQUIRE (res2.value<std::string>() == "456");
}