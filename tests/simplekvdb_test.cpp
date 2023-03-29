#include <thread>

#include <catch2/catch.hpp>

#include "simplekvdb/KvStore.hpp"
#include "testutil.hpp"

using namespace simplekvdb;

TEST_CASE("Example test case") {
    REQUIRE(1 + 1 == 2);
}

TEST_CASE("KvStore initialize") {
    DeleteFile df(0);
    KvStore kvStore(0, 100, false);
    int expectedCapacity = LOAD_FACTOR * 100;
    REQUIRE( expectedCapacity == kvStore.capacity());
    REQUIRE( 0 == kvStore.size());
}

TEST_CASE("KvStore set") {
    DeleteFile df(0);
    KvStore kvStore(0, 100, false);
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
    DeleteFile df(0);
    KvStore kvStore(0, 100, false);
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
    DeleteFile df(0);
    KvStore kvStore(0, 50, false);
    kvStore.set("abc", "def");
    kvStore.set("abc", "cde");
    auto [retCode, res] = kvStore.get("abc");
    REQUIRE (retCode == RetCode::SUCCESS);
    REQUIRE (res.value() == "cde");
}

TEST_CASE("KvStore get no exist") {
    DeleteFile df(0);
    KvStore kvStore(0, 100, false);
    auto [retCode, res] = kvStore.get("abc");
    REQUIRE (retCode == RetCode::DOES_NOT_EXIST);
    REQUIRE (res == std::nullopt);
}

TEST_CASE("KvStore set at capacity returns failure") {
    DeleteFile df(0);
    KvStore kvStore(0, 50, false);
    int cap = LOAD_FACTOR * 50;
    for (int i = 0; i < cap; i++) {
        kvStore.set(std::to_string(i), std::to_string(i));
    }
    REQUIRE (kvStore.size() == cap);
    
    auto retCode = kvStore.set("abc", "cde");
    REQUIRE (retCode == RetCode::FAILURE);
}

TEST_CASE("KvStore del normal success") {
    DeleteFile df(0);
    KvStore kvStore(0, 50, false);
    for (int i = 0; i < 10; i++) {
        kvStore.set(std::to_string(i), std::to_string(i));
    }
    auto retCode = kvStore.del("5");
    REQUIRE (retCode == RetCode::SUCCESS);
    REQUIRE (kvStore.size() == 9);
}

TEST_CASE("KvStore del no exist") {
    DeleteFile df(0);
    KvStore kvStore(0, 50, false);
    for (int i = 0; i < 10; i++) {
        kvStore.set(std::to_string(i), std::to_string(i));
    }
    auto retCode = kvStore.del("20");
    REQUIRE (retCode == RetCode::DOES_NOT_EXIST);
    REQUIRE (kvStore.size() == 10);
}

TEST_CASE("KvStore multi-threading set") {
    DeleteFile df(0);
    KvStore kvStore(0, 1000, false);
    constexpr int numThreads = 10;
    constexpr int numOps = 50;

    auto insertOp = [&kvStore](int start) {
        for (int i = 0; i < numOps; i++) {
            kvStore.set(std::to_string(start + i), "Value_" + std::to_string(start + i));
        }
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; i++) {
        threads.emplace_back(insertOp, i * numOps);
    }

    for (auto& t : threads) {
        t.join();
    }

    REQUIRE( kvStore.size() == numThreads * numOps);
}

TEST_CASE("KvStore multi-threading get") {
    DeleteFile df(0);
    KvStore kvStore(0, 1000, false);

    for (int i = 0; i < 500; i++) {
        kvStore.set(std::to_string(i), std::to_string(i));
    }

    constexpr int numThreads = 10;
    constexpr int numOps = 50;

    auto eraseOp = [&kvStore](int start) {
        for (int i = 0; i < numOps; ++i) {
            kvStore.del(std::to_string(i + start));
        }
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; i++) {
        threads.emplace_back(eraseOp, i * numOps);
    }

    for (auto& t : threads) {
        t.join();
    }

    REQUIRE( kvStore.size() == 0);
}