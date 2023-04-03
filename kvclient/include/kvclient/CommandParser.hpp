#pragma once
#include "Command.hpp"
#include <sstream>
#include <vector>

namespace kvclient {

    class CommandParser {

    public:

        static tParseCommand parseLine(const std::string& line);
        

    private:

        static std::vector<std::string> parseTokens(const std::string& line);

        static SetCommand parseSet(const std::vector<std::string>& tokens);

        static GetCommand parseGet(const std::vector<std::string>& tokens);

        static DelCommand parseDel(const std::vector<std::string>& tokens);

        static HSetCommand parseHSet(const std::vector<std::string>& tokens);

        static HGetCommand parseHGet(const std::vector<std::string>& tokens);

        static HDelCommand parseHDel(const std::vector<std::string>& tokens);

    };

    class InvalidCommandException : public std::runtime_error {
    public:
        explicit InvalidCommandException(const std::string& message)
            : std::runtime_error(message) {}

    };

}