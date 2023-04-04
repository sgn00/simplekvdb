#include <catch2/catch.hpp>

#include <chrono>
#include <iostream>

#include "simplekvdb/KvStore.hpp"
#include "testutil.hpp"

using namespace simplekvdb;

constexpr int kNumThreads = 10;
constexpr int kNumOperations = 100000;

// for each thread for kNumOperations
double test_function(KvStore& kvStore, int thread_id) {
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < kNumOperations; i++) {
        std::string key = std::to_string(thread_id * kNumOperations + i);
        kvStore.set(key, std::to_string(i));
        int value;
        kvStore.get(key);
        kvStore.del({key});
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    return static_cast<double>(duration) / 1000000.0; // Return time in seconds
}

// TEST_CASE("Multithreaded performance") {
//     DeleteFile df(2);
//     KvStore kvStore(2, 10000000, true);

//     std::vector<std::thread> threads;
//     std::vector<double> thread_times(kNumThreads);

//     for (int i = 0; i < kNumThreads; i++) {
//         threads.emplace_back([&kvStore, &thread_times, i]() {
//             thread_times[i] = test_function(kvStore, i);
//         });
//     }

//     double total_time = 0;
//     for (auto& t : threads) {
//         t.join();
//     }
//     for (const auto& time : thread_times) {
//         total_time += time;
//     }

//     double avg_time = total_time / kNumThreads;
//     int total_queries = kNumThreads * kNumOperations;
//     double qps = total_queries / avg_time;

//     std::cout << "Total time: " << total_time << " seconds" << std::endl;
//     std::cout << "Average time: " << avg_time << " seconds" << std::endl;
//     std::cout << "QPS: " << qps << std::endl;
// }
