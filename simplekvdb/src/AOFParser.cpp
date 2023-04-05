#include "simplekvdb/AOFParser.hpp"
#include "simplekvdb/LoggingUtil.hpp"

using namespace simplekvdb::aoflogging;

bool AOFParser::extractElement(const std::string& line, size_t& pos, int length, std::string& output) {
    if (pos + length > line.size()) {
        return false;
    }
    output = line.substr(pos, length);
    pos += length;
    return true;
}

bool AOFParser::extractField(const std::string& line, size_t& pos, std::string& field) {
    while (pos < line.size() && line[pos] != '|') {
        field += line[pos];
        pos++;
    }
    if (field.empty()) {
        return false;
    }

    return true;
}

bool AOFParser::extractLength(const std::string& line, size_t& pos, int& length) {
    std::string lengthStr;
    while (pos < line.size() && line[pos] != '|') {
        if (!isdigit(line[pos])) {
            return false;
        }
        lengthStr += line[pos];
        pos++;
    }
    if (lengthStr.empty()) {
        return false;
    }
    length = std::stoi(lengthStr);
    return true;
}

simplekvdb::SetCommand AOFParser::parseSetCommand(const std::string& line, size_t& pos) {
    std::string key, value;
    int keyLength, valueLength;

    if (!extractLength(line, pos, keyLength)) {
        throw AOFParseException("Could not extract key lenght in SET command");
    }
    pos++;  // Skip '|'

    if (!extractElement(line, pos, keyLength, key)) {
        throw AOFParseException("Could not extract key in SET command");
    }
    pos++;  // Skip '|'

    if (!extractLength(line, pos, valueLength)) {
        throw AOFParseException("Could not extract value length in SET command");
    }
    pos++;  // Skip '|'

    if (!extractElement(line, pos, valueLength, value)) {
        throw AOFParseException("Could not extract value in SET command");
    }

    return {key, value};
}

simplekvdb::DelCommand AOFParser::parseDelCommand(const std::string& line, size_t& pos) {
    std::vector<std::string> keys;
    while (pos < line.size()) {
        int elementLength;
        std::string element;
        if (!extractLength(line, pos, elementLength)) {
            throw AOFParseException("Could not extract key length in DEL command");
        }
        pos++;  // Skip '|'

        if (!extractElement(line, pos, elementLength, element)) {
            throw AOFParseException("Could not extract key value in DEL command");
        }
        pos++; // Skip '|'

        keys.push_back(element);       
    }
    return DelCommand{keys};
}

simplekvdb::HDelCommand AOFParser::parseHDelCommand(const std::string& line, size_t& pos) {
    std::string key;
    int keyLength;

    if (!extractLength(line, pos, keyLength)) {
        throw AOFParseException("Could not extract key length in HDEL command");
    }
    pos++;  // Skip '|'

    if (!extractElement(line, pos, keyLength, key)) {
        throw AOFParseException("Could not extract key in HDEL command");
    }
    pos++;  // Skip '|'

    std::vector<std::string> elements;

    // extract all fields
    while (pos < line.size()) {
        int elementLength;
        std::string element;
        if (!extractLength(line, pos, elementLength)) {
            throw AOFParseException("Could not extract field length in HDEL command");
        }
        pos++;  // Skip '|'

        if (!extractElement(line, pos, elementLength, element)) {
            throw AOFParseException("Could not extract field value in HDEL command");
        }
        pos++; // Skip '|'

        elements.push_back(element);
    }

    return {key, elements};
}

simplekvdb::HSetCommand AOFParser::parseHSetCommand(const std::string& line, size_t& pos) {
    std::string key;
    int keyLength;

    if (!extractLength(line, pos, keyLength)) {
        throw AOFParseException("Could not extract key length in HSET command");
    }
    pos++;  // Skip '|'

    if (!extractElement(line, pos, keyLength, key)) {
        throw AOFParseException("Could not extract key in HSET command");
    }
    pos++;  // Skip '|'

    std::vector<std::string> elements;

    // extract all fields
    while (pos < line.size()) {
        int elementLength;
        std::string element;
        if (!extractLength(line, pos, elementLength)) {
            throw AOFParseException("Could not extract field length in HSET command");
        }
        pos++;  // Skip '|'

        if (!extractElement(line, pos, elementLength, element)) {
            throw AOFParseException("Could not extract field value in HSET command");
        }
        pos++; // Skip '|'

        elements.push_back(element);
    }

    if (elements.size() % 2 != 0) {
        throw AOFParseException("Incorrect number of field values in HSET command");
    }

    int n = elements.size();
    std::vector<std::pair<std::string,std::string>> fieldValuePairs;
    for (int i = 0; i < n - 1; i += 2) {
        fieldValuePairs.push_back({std::move(elements[i]), std::move(elements[i + 1])});
    }

    return {key, fieldValuePairs};
}


simplekvdb::tCommand AOFParser::parseLine(const std::string& line) {
    size_t pos = 0;
    std::string command;

    if (!extractField(line, pos, command)) {
        throw AOFParseException("Could not parse command");
    }
    pos++;  // Skip '|'

    if (command == SET) {
        return parseSetCommand(line, pos);
    } else if (command == DEL) {
        return parseDelCommand(line, pos);
    } else if (command == HDEL) {
        return parseHDelCommand(line, pos);
    } else if (command == HSET) {
        return parseHSetCommand(line, pos);
    } else {
        throw AOFParseException("Unknown command");
    }
}


    
