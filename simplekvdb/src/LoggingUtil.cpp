#include <string>

#include "simplekvdb/LoggingUtil.hpp"

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

        // HSET|XX|KEY|XX|FIELD1|XX|VALUE1|XX|FIELD2|XX|VALUE2
        std::string stringifyHSetCommand(const std::string& key, const std::vector<std::pair<std::string,std::string>>& fieldValuePairs) {
            std::string firstPart = HSET + DELIM + std::to_string(key.size());
            std::string secondPart{};
            for (const auto& p : fieldValuePairs) {
                secondPart.append(DELIM);
                secondPart.append(std::to_string(p.first.size()));
                secondPart.append(DELIM);
                secondPart.append(p.first);
                secondPart.append(DELIM);
                secondPart.append(std::to_string(p.second.size()));
                secondPart.append(DELIM);
                secondPart.append(p.second);
            }
            return firstPart + secondPart;
        }

        // HDEL|XX|KEY|XX|FIELD1|XX|FIELD2
        std::string stringifyHDelCommand(const std::string& key, const std::vector<std::string>& fields) {
            std::string firstPart = HDEL + DELIM + std::to_string(key.size());
            std::string secondPart{};
            for (const auto& f : fields) {
                secondPart.append(DELIM);
                secondPart.append(std::to_string(f.size()));
                secondPart.append(DELIM);
                secondPart.append(f);
            }
            return firstPart + secondPart;
        }

        std::string getFileName(int DB_IDENTIFIER) {
            return "simplekvdb_" + std::to_string(DB_IDENTIFIER) + ".store";
        }

    }

}