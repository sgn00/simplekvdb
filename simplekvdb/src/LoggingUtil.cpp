#include <string>

namespace simplekvdb {

    namespace aoflogging {

        const std::string GET = "GET";
        const std::string DEL = "DEL";
        const std::string DELIM = "|";

        // COMMAND|XX|KEY|XX|VALUE
        std::string setCommand(const std::string& key, const std::string& value) {
            return GET + DELIM + std::to_string(key.size()) + DELIM + key + DELIM + std::to_string(value.size()) + DELIM + value;
        }

        // COMMAND|XX|KEY
        std::string delCommand(const std::string& key) {
            return DEL + DELIM + key;
        }

        std::string getFileName(int DB_IDENTIFIER) {
            return "simplekvdb_" + std::to_string(DB_IDENTIFIER) + ".store";
        }

    }

}