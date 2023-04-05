#pragma once
#include <stdexcept>
#include <string>

#include "Command.hpp"

namespace simplekvdb {

namespace aoflogging {

class AofParser {
 private:
  static bool extractElement(const std::string& line, size_t& pos, int length,
                             std::string& output);

  static bool extractField(const std::string& line, size_t& pos,
                           std::string& field);

  static bool extractLength(const std::string& line, size_t& pos, int& length);

  static simplekvdb::SetCommand parseSetCommand(const std::string& line,
                                                size_t& pos);

  static simplekvdb::DelCommand parseDelCommand(const std::string& line,
                                                size_t& pos);

  static simplekvdb::HDelCommand parseHDelCommand(const std::string& line,
                                                  size_t& pos);

  static simplekvdb::HSetCommand parseHSetCommand(const std::string& line,
                                                  size_t& pos);

 public:
  static simplekvdb::TCommand parseLine(const std::string& line);
};

class AOFParseException : public std::runtime_error {
 public:
  explicit AOFParseException(const std::string& message)
      : std::runtime_error(message) {}
};

}  // namespace aoflogging

}  // namespace simplekvdb