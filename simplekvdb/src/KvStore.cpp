#include "KvStore.hpp"

using namespace simplekvdb;

KvStore::KvStore(int ident, size_t numBuckets) 
    :   DB_IDENTIFIER(ident), 
        MAX_NUM_ELEMENTS(static_cast<int>(LOAD_FACTOR * numBuckets)), 
        buckets(numBuckets) {}

size_t KvStore::size() const {
    return static_cast<size_t>(numElements);
}

size_t KvStore::capacity() const {
    return MAX_NUM_ELEMENTS;
}

KvStore::Bucket& KvStore::getBucket(const std::string& key) {
    size_t index = hasher(key) % buckets.size();
    return buckets[index];
}

const KvStore::Bucket& KvStore::getBucket(const std::string& key) const {
    size_t index = hasher(key) % buckets.size();
    return buckets[index];
}

bool KvStore::isFull() const {
    return numElements >= MAX_NUM_ELEMENTS;
}


RetCode KvStore::set(const std::string& key, const std::string& value) {
    Bucket& bucket = getBucket(key);
    std::unique_lock lock(bucket.mutex);
    bool inserted = false;
    for (auto& kv : bucket.data) {
        if (kv.first == key) {
            kv.second = value;
            inserted = true;
            break;
        }
    }

    if (inserted) {
        return RetCode::SUCCESS_AND_EXISTED;
    } else if (!isFull()) {
        bucket.data.emplace_back(key, value);
        ++numElements;
        return RetCode::SUCCESS;
    } else {
        return RetCode::FAILURE;
    }
}

std::pair<RetCode,std::optional<std::string>> KvStore::get(const std::string& key) const {
    const Bucket& bucket = getBucket(key);
    std::shared_lock lock(bucket.mutex);

    for (const auto& kv : bucket.data) {
        if (kv.first == key) {
            if (std::holds_alternative<std::string>(kv.second)) {
                return std::make_pair(RetCode::SUCCESS,std::get<std::string>(kv.second));
            } else {
                return std::make_pair(RetCode::WRONG_TYPE, std::nullopt);
            }
        }
    }
    return std::make_pair(RetCode::DOES_NOT_EXIST,std::nullopt);
}

RetCode KvStore::del(const std::string& key) {
    Bucket& bucket = getBucket(key);
    std::unique_lock lock(bucket.mutex);

    auto initialSize = bucket.data.size();
    bucket.data.remove_if(
        [&key = key](const auto& kv) { 
            return kv.first == key; 
        }
    );

    auto removedCount = initialSize - bucket.data.size();
    numElements -= removedCount;
    if (removedCount == 0) {
        return RetCode::DOES_NOT_EXIST;
    }
    return RetCode::SUCCESS;
}