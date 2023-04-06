#include "simplekvdb/AofLoader.hpp"
#include "simplekvdb/KvStore.hpp"
#include "simplekvdb/LoggingUtil.hpp"

using namespace simplekvdb;

KvStore::KvStore(int ident, size_t numBuckets, bool loggingEnabled)
    : DB_IDENTIFIER(ident),
      MAX_NUM_ELEMENTS(static_cast<int>(LOAD_FACTOR * numBuckets)),
      loggingEnabled(loggingEnabled),
      buckets(numBuckets) {
  if (loggingEnabled) {
    logWriter.emplace(aoflogging::getFileName(DB_IDENTIFIER));
  }

  aoflogging::AofLoader::loadAndExecute(*this);
}

size_t KvStore::size() const {
  return static_cast<size_t>(numElements.load(std::memory_order_acquire));
}

size_t KvStore::capacity() const { return MAX_NUM_ELEMENTS; }

int KvStore::getIdent() const { return DB_IDENTIFIER; }

KvStore::Bucket &KvStore::getBucket(const std::string &key) {
  size_t index = hasher(key) % buckets.size();
  return buckets[index];
}

const KvStore::Bucket &KvStore::getBucket(const std::string &key) const {
  size_t index = hasher(key) % buckets.size();
  return buckets[index];
}

bool KvStore::isFull() const {
  return numElements.load(std::memory_order_acquire) >= MAX_NUM_ELEMENTS;
}

void KvStore::setLoggingEnabled(bool enabled) { loggingEnabled = enabled; }

bool KvStore::isLoggingEnabled() { return loggingEnabled; }

Result KvStore::set(const std::string &key, const std::string &value) {
  Bucket &bucket = getBucket(key);
  std::unique_lock lock(bucket.mutex);

  if (loggingEnabled) {
    logWriter.value().log(aoflogging::stringifySetCommand(key, value));
  }

  bool inserted = false;
  for (auto &kv : bucket.data) {
    if (kv.first == key) {
      kv.second = value;
      inserted = true;
      break;
    }
  }

  if (inserted) {
    return Result{Result::Status::OK};
  } else {
    std::unique_lock<std::mutex> insertionLock(insertionMutex); // make check and increment op atomic
    if (!isFull()) {
      bucket.data.emplace_back(key, value);
      numElements.fetch_add(1, std::memory_order_release);
      return Result{Result::Status::OK};
    } else {
      return Result{Result::Status::Error, Result::ErrorCode::DBFull};
    }
  } 
}

Result KvStore::get(const std::string &key) const {
  const Bucket &bucket = getBucket(key);
  std::shared_lock lock(bucket.mutex);

  for (const auto &kv : bucket.data) {
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

Result KvStore::del(const std::vector<std::string> &keys) {
  int removedCount = 0;

  if (loggingEnabled) {
    logWriter.value().log(aoflogging::stringifyDelCommand(keys));
  }

  for (const auto &key : keys) {
    Bucket &bucket = getBucket(key);
    std::unique_lock lock(bucket.mutex);

    auto initialSize = bucket.data.size();
    bucket.data.remove_if([&key](const auto &kv) { return kv.first == key; });

    removedCount += initialSize - bucket.data.size();
    numElements.fetch_sub(initialSize - bucket.data.size(),
                          std::memory_order_release);
  }

  return Result{Result::Status::OK, removedCount};
}
Result KvStore::hset(
    const std::string &key,
    const std::vector<std::pair<std::string, std::string>> &fieldValuePairs) {
  Bucket &bucket = getBucket(key);
  std::unique_lock lock(bucket.mutex);

  if (loggingEnabled) {
    logWriter.value().log(
        aoflogging::stringifyHsetCommand(key, fieldValuePairs));
  }

  bool inserted = false;
  for (auto &kv : bucket.data) {
    if (kv.first == key) {
      if (std::holds_alternative<std::unordered_map<std::string, std::string>>(
              kv.second)) {
        auto &umap =
            std::get<std::unordered_map<std::string, std::string>>(kv.second);
        for (const auto &[k, v] : fieldValuePairs) {
          umap[k] = v;
        }
        inserted = true;
        break;
      } else {
        return Result{Result::Status::Error, Result::ErrorCode::WrongType};
      }
    }
  }

  if (inserted) {
    return Result{Result::Status::OK, static_cast<int>(fieldValuePairs.size())};
  } else {
    std::unique_lock<std::mutex> insertionLock(insertionMutex); // make check and increment op atomic
    if (!isFull()) {
      std::unordered_map<std::string, std::string> umap;
      for (const auto &[k, v] : fieldValuePairs) {
        umap[k] = v;
      }
      bucket.data.emplace_back(key, umap);
      numElements.fetch_add(1, std::memory_order_release);
      return Result{Result::Status::OK, static_cast<int>(fieldValuePairs.size())};
    } else {
      return Result{Result::Status::Error, Result::ErrorCode::DBFull};
    }
  } 
}

Result KvStore::hget(const std::string &key, const std::string &field) const {
  const Bucket &bucket = getBucket(key);
  std::shared_lock lock(bucket.mutex);

  for (const auto &kv : bucket.data) {
    if (kv.first == key) {
      if (std::holds_alternative<std::unordered_map<std::string, std::string>>(
              kv.second)) {
        const auto &umap =
            std::get<std::unordered_map<std::string, std::string>>(kv.second);
        if (umap.count(field)) {
          return Result{Result::Status::OK, umap.at(field)};
        } else {
          return Result{Result::Status::Error,
                        Result::ErrorCode::FieldNotFound};
        }
      } else {
        return Result{Result::Status::Error, Result::ErrorCode::WrongType};
      }
    }
  }
  return Result{Result::Status::Error, Result::ErrorCode::KeyNotFound};
}

Result KvStore::hdel(const std::string &key,
                     const std::vector<std::string> &fields) {
  Bucket &bucket = getBucket(key);
  std::unique_lock lock(bucket.mutex);

  if (loggingEnabled) {
    logWriter.value().log(aoflogging::stringifyHdelCommand(key, fields));
  }

  for (auto &kv : bucket.data) {
    if (kv.first == key) {
      if (std::holds_alternative<std::unordered_map<std::string, std::string>>(
              kv.second)) {
        int count = 0;
        for (const auto &field : fields) {
          auto &umap =
              std::get<std::unordered_map<std::string, std::string>>(kv.second);
          if (umap.erase(field)) {
            ++count;
          }
        }
        return Result{Result::Status::OK, count};
      } else {
        return Result{Result::Status::Error, Result::ErrorCode::WrongType};
      }
    }
  }

  return Result{Result::Status::Error, Result::ErrorCode::KeyNotFound};
}