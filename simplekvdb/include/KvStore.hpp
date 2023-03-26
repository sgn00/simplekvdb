#pragma once
#include <variant>
#include <list>
#include <shared_mutex>
#include <mutex>
#include <string>
#include <optional>
#include <vector>
#include <atomic>
#include <unordered_map>

#include "RetCode.hpp"
#include "LogWriter.hpp"

namespace simplekvdb {

    using TValueVariant = std::variant<std::string,std::unordered_map<std::string,std::string>>;

    const double LOAD_FACTOR = 0.75;

class KvStore {

private:

    struct Bucket {
        std::list<std::pair<std::string,TValueVariant>> data;
        mutable std::shared_mutex mutex;
    };

    std::vector<Bucket> buckets;
    std::hash<std::string> hasher;

    const int DB_IDENTIFIER;

    const int MAX_NUM_ELEMENTS;

    LogWriter logWriter;

    std::atomic<size_t> numElements {0};

    Bucket& getBucket(const std::string& key);

    const Bucket& getBucket(const std::string& key) const;

    bool isFull() const;

public:

    explicit KvStore(int ident, size_t numBuckets);

    size_t size() const;

    size_t capacity() const;

    RetCode set(const std::string& key, const std::string& value);

    std::pair<RetCode,std::optional<std::string>> get(const std::string& key) const;

    RetCode del(const std::string& key);

    // bool hset(const std::string& key, const std::vector<std::pair<std::string,std::string>>& fieldValuePairs);

    // std::optional<std::string> hget(const std::string& key, const std::string& field);

};

}