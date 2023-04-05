#include "simplekvdb/LoggingUtil.hpp"

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <string>

namespace simplekvdb {

namespace aoflogging {

// COMMAND|XX|KEY|XX|VALUE
std::string stringifySetCommand(const std::string &key,
                                const std::string &value) {
  return fmt::format("{}{}{}{}{}{}{}{}{}", SET, DELIM, key.size(), DELIM, key,
                     DELIM, value.size(), DELIM, value);
}

// COMMAND|XX|KEY|...
std::string stringifyDelCommand(const std::vector<std::string> &keys) {
  fmt::memory_buffer buffer;
  fmt::format_to(std::back_inserter(buffer), "{}", DEL);

  for (const auto &k : keys) {
    fmt::format_to(std::back_inserter(buffer), "{}{}{}{}", DELIM, k.size(),
                   DELIM, k);
  }

  return fmt::to_string(buffer);
}

// HSET|XX|KEY|XX|FIELD1|XX|VALUE1|XX|FIELD2|XX|VALUE2
std::string stringifyHsetCommand(
    const std::string &key,
    const std::vector<std::pair<std::string, std::string>> &fieldValuePairs) {
  fmt::memory_buffer buffer;
  fmt::format_to(std::back_inserter(buffer), "{}{}{}{}{}", HSET, DELIM,
                 key.size(), DELIM, key);

  for (const auto &p : fieldValuePairs) {
    fmt::format_to(std::back_inserter(buffer), "{}{}{}{}{}{}{}{}", DELIM,
                   p.first.size(), DELIM, p.first, DELIM, p.second.size(),
                   DELIM, p.second);
  }

  return fmt::to_string(buffer);
}

// HDEL|XX|KEY|XX|FIELD1|XX|FIELD2
std::string stringifyHdelCommand(const std::string &key,
                                 const std::vector<std::string> &fields) {
  fmt::memory_buffer buffer;
  fmt::format_to(std::back_inserter(buffer), "{}{}{}{}{}", HDEL, DELIM,
                 key.size(), DELIM, key);

  for (const auto &f : fields) {
    fmt::format_to(std::back_inserter(buffer), "{}{}{}{}", DELIM, f.size(),
                   DELIM, f);
  }

  return fmt::to_string(buffer);
}

std::string getFileName(int DB_IDENTIFIER) {
  return fmt::format("simplekvdb_{}.store", DB_IDENTIFIER);
}

}  // namespace aoflogging

}  // namespace simplekvdb