#include <fmt/core.h>

#include <fstream>

#include "simplekvdb/AofLoader.hpp"
#include "simplekvdb/AofParser.hpp"
#include "simplekvdb/LoggingUtil.hpp"

namespace simplekvdb {

void aoflogging::AofLoader::execute(KvStore &kvStore, TCommand command) {
  if (std::holds_alternative<SetCommand>(command)) {
    auto c = std::get<SetCommand>(command);
    kvStore.set(c.key, c.value);
  } else if (std::holds_alternative<DelCommand>(command)) {
    auto c = std::get<DelCommand>(command);
    kvStore.del(c.keys);
  } else if (std::holds_alternative<HDelCommand>(command)) {
    auto c = std::get<HDelCommand>(command);
    kvStore.hdel(c.key, c.fields);
  } else if (std::holds_alternative<HSetCommand>(command)) {
    auto c = std::get<HSetCommand>(command);
    kvStore.hset(c.key, c.fieldValuePairs);
  }
}

bool aoflogging::AofLoader::loadAndExecute(KvStore &kvStore) {
  bool loggingEnabledState = kvStore.isLoggingEnabled();
  kvStore.setLoggingEnabled(false);
  std::string filePath = getFileName(kvStore.getIdent());

  std::ifstream inputFile(filePath);

  // Check if the file is successfully opened
  if (!inputFile.is_open()) {
    return false;
  }

  AofParser parser;

  int lineNumber = 0;
  std::string line;
  while (std::getline(inputFile, line)) {
    lineNumber++;
    try {
      simplekvdb::TCommand command = parser.parseLine(line);
      execute(kvStore, command);
    } catch (const AOFParseException &e) {
      fmt::print(stderr, "AOFParseException at line {}: {}\n", lineNumber,
                 e.what());
    }
  }

  // Close the file
  inputFile.close();

  kvStore.setLoggingEnabled(loggingEnabledState);

  return true;
}

}