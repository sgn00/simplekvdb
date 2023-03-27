#pragma once
#include <string>
#include "Command.hpp"

namespace simplekvdb {

    namespace aoflogging {

        class AOFParser {
        
        private:

            static bool extractElement(const std::string& line, size_t& pos, int length, std::string& output);

            static bool extractField(const std::string& line, size_t& pos, std::string& field);

            static bool extractLength(const std::string& line, size_t& pos, int& length);

            static simplekvdb::SetCommand parseSetCommand(const std::string& line, size_t& pos);

            static simplekvdb::DelCommand parseDelCommand(const std::string& line, size_t& pos);

        public:

            static simplekvdb::tCommand parseLine(const std::string& line);


        };






    }

}