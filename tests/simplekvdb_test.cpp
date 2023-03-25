#include <catch2/catch.hpp>
#include "KvStore.hpp"

using namespace simplekvdb;

TEST_CASE("Example test case") {
    REQUIRE(1 + 1 == 2);
}

TEST_CASE("KvStore initialize") {
    KvStore kvStore(0, 100);
    int expectedCapacity = LOAD_FACTOR * 100;
    REQUIRE( expectedCapacity == kvStore.capacity());
    REQUIRE( 0 == kvStore.size());
}

TEST_CASE("KvStore set") {
    KvStore kvStore(0, 100);
    auto res = kvStore.set("abc", "def");
    REQUIRE(res == RetCode::SUCCESS);
    auto res2 = kvStore.set("cde", "fgh");
    REQUIRE(res == RetCode::SUCCESS);
    auto res3 = kvStore.set("cde", "ghi");
    REQUIRE(res3 == RetCode::SUCCESS_AND_EXISTED);
    auto currSize = kvStore.size();
    REQUIRE(currSize == 2);
}

TEST_CASE("KvStore set and get") {
    KvStore kvStore(0, 100);
    kvStore.set("abc", "def");
    kvStore.set("cde", "ghi");
    auto [retCode, res] = kvStore.get("abc");
    REQUIRE (res.value() == "def");
    REQUIRE (retCode == RetCode::SUCCESS);
    auto [retCode2, res2] = kvStore.get("cde");
    REQUIRE( res2.value() == "ghi");
    REQUIRE (retCode2 == RetCode::SUCCESS);
}

TEST_CASE("KvStore set overwrite and get") {
    KvStore kvStore(0, 50);
    kvStore.set("abc", "def");
    kvStore.set("abc", "cde");
    auto [retCode, res] = kvStore.get("abc");
    REQUIRE (retCode == RetCode::SUCCESS);
    REQUIRE (res.value() == "cde");
}

TEST_CASE("KvStore get no exist") {
    KvStore kvStore(0, 100);
    auto [retCode, res] = kvStore.get("abc");
    REQUIRE (retCode == RetCode::DOES_NOT_EXIST);
    REQUIRE (res == std::nullopt);
}

TEST_CASE("KvStore set at capacity returns failure") {
    KvStore kvStore(0, 50);
    int cap = LOAD_FACTOR * 50;
    for (int i = 0; i < cap; i++) {
        kvStore.set(std::to_string(i), std::to_string(i));
    }
    REQUIRE (kvStore.size() == cap);
    
    auto retCode = kvStore.set("abc", "cde");
    REQUIRE (retCode == RetCode::FAILURE);
}

TEST_CASE("KvStore del normal success") {
    KvStore kvStore(0, 50);
    for (int i = 0; i < 10; i++) {
        kvStore.set(std::to_string(i), std::to_string(i));
    }
    auto retCode = kvStore.del("5");
    REQUIRE (retCode == RetCode::SUCCESS);
    REQUIRE (kvStore.size() == 9);
}

TEST_CASE("KvStore del no exist") {
    KvStore kvStore(0, 50);
    for (int i = 0; i < 10; i++) {
        kvStore.set(std::to_string(i), std::to_string(i));
    }
    auto retCode = kvStore.del("20");
    REQUIRE (retCode == RetCode::DOES_NOT_EXIST);
    REQUIRE (kvStore.size() == 10);
}