#pragma once
#include <string>

namespace simplekvdb {

    namespace aoflogging {

        const std::string SET = "SET";
        const std::string DEL = "DEL";
        const std::string DELIM = "|";

        // COMMAND|XX|KEY|XX|VALUE
        std::string stringifySetCommand(const std::string& key, const std::string& value);

        // COMMAND|XX|KEY
        std::string stringifyDelCommand(const std::string& key);

        std::string getFileName(int DB_IDENTIFIER);


    }

}