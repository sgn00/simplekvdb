#include <catch2/catch.hpp>

#include "simplekvdb/KvStore.hpp"
#include "testutil.hpp"

using namespace simplekvdb;

TEST_CASE("HSET basic success test") {
    DeleteFile df(20);
    KvStore kvStore(20, 100, false);
    auto [retCode, numFieldsSet] = kvStore.hset("hhh", {{"abc", "123"}, {"ghi", "456"}});
    REQUIRE (retCode == RetCode::SUCCESS);
    REQUIRE (numFieldsSet == 2);
}

TEST_CASE("HSET, HGET basic success test") {
    DeleteFile df(20);
    KvStore kvStore(20, 100, false);
    kvStore.hset("hhh", {{"abc", "123"}, {"ghi", "456"}});
    auto [retCode, res] = kvStore.hget("hhh", "abc");
    REQUIRE (retCode == RetCode::SUCCESS);
    REQUIRE (res.value() == "123");

    auto [retCode2, res2] = kvStore.hget("hhh", "ghi");
    REQUIRE (retCode2 == RetCode::SUCCESS);
    REQUIRE (res2.value() == "456");
}

TEST_CASE("HSET, HDEL basic success test") {
    DeleteFile df(20);
    KvStore kvStore(20, 100, false);
    kvStore.hset("hhh", {{"abc", "123"}, {"ghi", "456"}});
    auto [retCode, res] = kvStore.hdel("hhh", {"abc"});
    REQUIRE (retCode == RetCode::SUCCESS);
    REQUIRE (res == 1);
}

TEST_CASE("HSET, HDEL, HGET basic test") {
    DeleteFile df(20);
    KvStore kvStore(20, 100, false);
    kvStore.hset("hhh", {{"abc", "123"}, {"ghi", "456"}});
    kvStore.hdel("hhh", {"abc"});
    auto [retCode, res] = kvStore.hget("hhh", "abc");
    REQUIRE (retCode == RetCode::FAILURE);
    REQUIRE (res == std::nullopt);
    auto [retCode2, res2] = kvStore.hget("hhh", "ghi");
    REQUIRE (retCode2 == RetCode::SUCCESS);
    REQUIRE (res2.value() == "456");
}