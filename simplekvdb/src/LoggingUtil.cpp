#include <string>

#include "LoggingUtil.hpp"

namespace simplekvdb {

    namespace aoflogging {

        // COMMAND|XX|KEY|XX|VALUE
        std::string stringifySetCommand(const std::string& key, const std::string& value) {
            return SET + DELIM + std::to_string(key.size()) + DELIM + key + DELIM + std::to_string(value.size()) + DELIM + value;
        }

        // COMMAND|XX|KEY
        std::string stringifyDelCommand(const std::string& key) {
            return DEL + DELIM + std::to_string(key.size()) + DELIM + key;
        }

        std::string getFileName(int DB_IDENTIFIER) {
            return "simplekvdb_" + std::to_string(DB_IDENTIFIER) + ".store";
        }

    }

}