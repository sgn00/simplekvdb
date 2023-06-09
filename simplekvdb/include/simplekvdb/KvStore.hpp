#pragma once
#include <atomic>
#include <list>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "LogWriter.hpp"
#include "Result.hpp"

namespace simplekvdb {

namespace aoflogging {
class AofLoader;
}

using TValueVariant =
    std::variant<std::string, std::unordered_map<std::string, std::string>>;

const double LOAD_FACTOR = 0.75;

class KvStore {
 private:
  struct Bucket {
    std::list<std::pair<std::string, TValueVariant>> data;
    mutable std::shared_mutex mutex;
  };

  std::vector<Bucket> buckets;
  std::hash<std::string> hasher;

  const int DB_IDENTIFIER;

  const int MAX_NUM_ELEMENTS;

  std::optional<LogWriter> logWriter;

  bool loggingEnabled;

  std::atomic<size_t> numElements{0};

  std::mutex insertionMutex;

  Bucket& getBucket(const std::string& key);

  const Bucket& getBucket(const std::string& key) const;

  bool isFull() const;

  void setLoggingEnabled(bool enabled);

  friend class aoflogging::AofLoader;

 public:
  explicit KvStore(int ident, size_t numBuckets, bool loggingEnabled);

  size_t size() const;

  size_t capacity() const;

  int getIdent() const;

  bool isLoggingEnabled();

  Result set(const std::string& key, const std::string& value);

  Result get(const std::string& key) const;

  Result del(const std::vector<std::string>& keys);

  Result hset(
      const std::string& key,
      const std::vector<std::pair<std::string, std::string>>& fieldValuePairs);

  Result hget(const std::string& key, const std::string& field) const;

  Result hdel(const std::string& key, const std::vector<std::string>& fields);
};

}  // namespace simplekvdb