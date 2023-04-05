#pragma once
#include <string>
#include <variant>
#include <vector>

namespace kvclient {

const std::string GET = "GET";
const std::string SET = "SET";
const std::string DEL = "DEL";
const std::string HSET = "HSET";
const std::string HGET = "HGET";
const std::string HDEL = "HDEL";
const std::string QUIT = "QUIT";

struct SetCommand {
  std::string key;
  std::string value;
};

struct DelCommand {
  std::vector<std::string> keys;
};

struct GetCommand {
  std::string key;
};

struct HSetCommand {
  std::string key;
  std::vector<std::pair<std::string, std::string>> fieldValuePairs;
};

struct HGetCommand {
  std::string key;
  std::string field;
};

struct HDelCommand {
  std::string key;
  std::vector<std::string> fields;
};

struct QuitCommand {};

using TParseCommand =
    std::variant<SetCommand, DelCommand, GetCommand, HSetCommand, HGetCommand,
                 HDelCommand, QuitCommand>;
}  // namespace kvclient