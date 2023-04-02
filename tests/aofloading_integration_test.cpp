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
            kvStore.del(std::to_string(i));
        }
    }

    int numExpected = 10 - 5;

    KvStore kvStore(1, 100, true);
    REQUIRE (kvStore.size() == numExpected);
    for (int i = 6; i < 9; i++) {
        auto res = kvStore.get(std::to_string(i));
        REQUIRE (res.status() == Result::Status::OK);
    }

}