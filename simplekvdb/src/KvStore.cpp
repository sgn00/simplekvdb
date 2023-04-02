#include "simplekvdb/KvStore.hpp"
#include "simplekvdb/LoggingUtil.hpp"
#include "simplekvdb/AOFLoader.hpp"

using namespace simplekvdb;

KvStore::KvStore(int ident, size_t numBuckets, bool loggingEnabled) 
    :   DB_IDENTIFIER(ident), 
        MAX_NUM_ELEMENTS(static_cast<int>(LOAD_FACTOR * numBuckets)),
        logWriter(aoflogging::getFileName(DB_IDENTIFIER)),
        loggingEnabled(loggingEnabled),
        buckets(numBuckets) {

    aoflogging::AOFLoader::loadAndExecute(*this);

}

size_t KvStore::size() const {
    return static_cast<size_t>(numElements);
}

size_t KvStore::capacity() const {
    return MAX_NUM_ELEMENTS;
}

int KvStore::getIdent() const {
    return DB_IDENTIFIER;
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

void KvStore::setLoggingEnabled(bool enabled) {
    loggingEnabled = enabled;
}

bool KvStore::isLoggingEnabled() {
    return loggingEnabled;
}

Result KvStore::set(const std::string& key, const std::string& value) {
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

    if (loggingEnabled) {
        logWriter.log(aoflogging::stringifySetCommand(key, value));
    }

    if (inserted) {
        return Result{Result::Status::OK};
    } else if (!isFull()) {
        bucket.data.emplace_back(key, value);
        ++numElements;
        return Result{Result::Status::OK};
    } else {
        return Result{Result::Status::Error, Result::ErrorCode::DBFull};
    }
}

Result KvStore::get(const std::string& key) const {
    const Bucket& bucket = getBucket(key);
    std::shared_lock lock(bucket.mutex);

    for (const auto& kv : bucket.data) {
        if (kv.first == key) {
            if (std::holds_alternative<std::string>(kv.second)) {
                return Result{Result::Status::OK, std::get<std::string>(kv.second)};
            } else {
                return Result{Result::Status::Error, Result::ErrorCode::WrongType};
            }
        }
    }
    return Result{Result::Status::Error, Result::ErrorCode::KeyNotFound};
}

Result KvStore::del(const std::string& key) {
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

    if (loggingEnabled) {
        logWriter.log(aoflogging::stringifyDelCommand(key));
    }

    return Result{Result::Status::OK, static_cast<int>(removedCount)};
}

Result KvStore::hset(const std::string& key, const std::vector<std::pair<std::string,std::string>>& fieldValuePairs) {
    Bucket& bucket = getBucket(key);
    std::unique_lock lock(bucket.mutex);
    bool inserted = false;
    for (auto& kv : bucket.data) {
        if (kv.first == key) {
            if (std::holds_alternative<std::unordered_map<std::string,std::string>>(kv.second)) {
                std::unordered_map<std::string,std::string> umap;
                for (const auto& [k,v] : fieldValuePairs) {
                    umap[k] = v;
                }
                kv.second = umap;
                inserted = true;
                break;
            } else {
                return Result{Result::Status::Error, Result::ErrorCode::WrongType};
            }
        }
    }

    // if (loggingEnabled) {
    //     logWriter.log(aoflogging::stringifySetCommand(key, value));
    // }

    if (inserted) {
        return Result{Result::Status::OK, static_cast<int>(fieldValuePairs.size())};
    } else if (!isFull()) {
        std::unordered_map<std::string,std::string> umap;
        for (const auto& [k,v] : fieldValuePairs) {
            umap[k] = v;
        }
        bucket.data.emplace_back(key, umap);
        ++numElements;
        return Result{Result::Status::OK, static_cast<int>(fieldValuePairs.size())};
    } else {
        return Result{Result::Status::Error, Result::ErrorCode::DBFull};
    }
}

Result KvStore::hget(const std::string& key, const std::string& field) const {
    const Bucket& bucket = getBucket(key);
    std::shared_lock lock(bucket.mutex);

    for (const auto& kv : bucket.data) {
        if (kv.first == key) {
            if (std::holds_alternative<std::unordered_map<std::string,std::string>>(kv.second)) {
                const auto& umap = std::get<std::unordered_map<std::string,std::string>>(kv.second);
                if (umap.count(field)) {
                    return Result{Result::Status::OK, umap.at(field)};
                } else {
                    return Result{Result::Status::Error, Result::ErrorCode::FieldNotFound};
                }
            } else {
                return Result{Result::Status::Error, Result::ErrorCode::WrongType};
            }
        }
    }
    return Result{Result::Status::Error, Result::ErrorCode::KeyNotFound};
}

Result KvStore::hdel(const std::string& key, const std::vector<std::string>& fields) {
    Bucket& bucket = getBucket(key);
    std::unique_lock lock(bucket.mutex);

    for (auto& kv : bucket.data) {
        if (kv.first == key) {
            if (std::holds_alternative<std::unordered_map<std::string,std::string>>(kv.second)) {
                int count = 0;
                for (const auto& field : fields) {
                    auto& umap = std::get<std::unordered_map<std::string,std::string>>(kv.second);
                    if (umap.erase(field)) {
                        ++count;
                    }

                    // if (loggingEnabled) {
                    //     logWriter.log(aoflogging::stringifyDelCommand(key));
                    // }

                    return Result{Result::Status::OK, count};
                }
            } else {
                return Result{Result::Status::Error, Result::ErrorCode::WrongType};
            }
        }
    }

    return Result{Result::Status::Error, Result::ErrorCode::KeyNotFound};
}