#include "kvserver/Server.hpp"

#include <fmt/ranges.h>

namespace kvserver {
Server::Server(int port, bool loggingEnabled)
    : port(port), kvStore(DB_IDENTIFIER, size, loggingEnabled), server(port) {
  fmt::print("Using port: {}\n", port);
  fmt::print("Logging enabled: {}\n", loggingEnabled ? "true" : "false");
}

void Server::start() {
  bindFunctions();
  fmt::print("Running server...\n");
  server.run();
}

void Server::bindFunctions() {
  server.bind("SET", [this](const std::string &key, const std::string &value) {
    return set(key, value);
  });

  server.bind("GET", [this](const std::string &key) { return get(key); });

  server.bind("DEL", [this](const std::vector<std::string> &keys) {
    return del(keys);
  });

  server.bind("HGET", [this](const std::string &key, const std::string &field) {
    return hget(key, field);
  });

  server.bind(
      "HSET",
      [this](const std::string &key,
             const std::vector<std::pair<std::string, std::string>>
                 &fieldValuePairs) { return hset(key, fieldValuePairs); });

  server.bind("HDEL", [this](const std::string &key,
                             const std::vector<std::string> &fields) {
    return hdel(key, fields);
  });
}

CommResult Server::set(const std::string &key, const std::string &value) {
  fmt::print("SET called: {},{}\n", key, value);
  auto res = kvStore.set(key, value);
  return convertToCommResult(res);
}

CommResult Server::get(const std::string &key) {
  fmt::print("GET called: {}\n", key);
  auto res = kvStore.get(key);
  return convertToCommResult(res);
}

CommResult Server::del(const std::vector<std::string> &keys) {
  fmt::print("DEL called: {}\n", keys);
  auto res = kvStore.del(keys);
  return convertToCommResult(res);
}

CommResult Server::hget(const std::string &key, const std::string &field) {
  fmt::print("HGET called: {}\n", key);
  auto res = kvStore.hget(key, field);
  return convertToCommResult(res);
}

CommResult Server::hset(
    const std::string &key,
    const std::vector<std::pair<std::string, std::string>> &fieldValuePairs) {
  fmt::print("HSET called: {}\n", key);
  auto res = kvStore.hset(key, fieldValuePairs);
  return convertToCommResult(res);
}

CommResult Server::hdel(const std::string &key,
                        const std::vector<std::string> &fields) {
  fmt::print("HDEL called: {}\n", key);
  auto res = kvStore.hdel(key, fields);
  return convertToCommResult(res);
}

CommResult Server::convertToCommResult(const simplekvdb::Result &r) {
  int field1, field2, field4 = -1;
  std::string field3;

  field1 = static_cast<int>(r.status());
  if (r.status() == simplekvdb::Result::Status::Error) {
    field2 = static_cast<int>(r.error_code());
  } else {
    if (r.holds_type<std::string>()) {
      field3 = r.value<std::string>();
    } else if (r.holds_type<int>()) {
      field4 = r.value<int>();
    }
  }
  return std::make_tuple(field1, field2, field3, field4);
}
}  // namespace kvserver