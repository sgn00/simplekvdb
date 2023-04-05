#pragma once
#include <tuple>

#include "rpc/server.h"
#include "simplekvdb/KvStore.hpp"

namespace kvserver {

using CommResult = std::tuple<int, int, std::string, int>;

class Server {
 public:
  Server(int port, bool loggingEnabled);

  void start();

 private:
  const int size = 10'000'000;

  const int port;

  const int DB_IDENTIFIER = 1;

  simplekvdb::KvStore kvStore;

  rpc::server server;

  void bindFunctions();

  static CommResult convertToCommResult(const simplekvdb::Result &r);

  CommResult set(const std::string &key, const std::string &value);

  CommResult get(const std::string &key);

  CommResult del(const std::vector<std::string> &keys);

  CommResult hget(const std::string &key, const std::string &field);

  CommResult hset(
      const std::string &key,
      const std::vector<std::pair<std::string, std::string>> &fieldValuePairs);

  CommResult hdel(const std::string &key,
                  const std::vector<std::string> &fields);
};

}  // namespace kvserver