#include "simplekvdb/AOFLoader.hpp"
#include "simplekvdb/AOFParser.hpp"
#include "simplekvdb/LoggingUtil.hpp"
#include <fstream>
#include <iostream>

using namespace simplekvdb;

void aoflogging::AOFLoader::execute(KvStore& kvStore, tCommand command) {
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


bool aoflogging::AOFLoader::loadAndExecute(KvStore& kvStore) {
    bool loggingEnabledState = kvStore.isLoggingEnabled();
    kvStore.setLoggingEnabled(false);
    std::string filePath = getFileName(kvStore.getIdent());

    std::ifstream inputFile(filePath);

    // Check if the file is successfully opened
    if (!inputFile.is_open()) {
        return false;
    }

    AOFParser parser;

    int lineNumber = 0;
    std::string line;
    while (std::getline(inputFile, line)) {
        lineNumber++;
        try {
            simplekvdb::tCommand command = parser.parseLine(line);
            execute(kvStore, command);
        } catch (const AOFParseException& e) {
            std::cerr << "AOFParseException at line " << lineNumber << ": " << e.what() << std::endl;
        }
    }

    // Close the file
    inputFile.close();

    kvStore.setLoggingEnabled(loggingEnabledState);

    return true;
}
