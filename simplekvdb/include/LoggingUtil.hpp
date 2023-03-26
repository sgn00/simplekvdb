#pragma once
#include <string>

namespace simplekvdb {

    namespace aoflogging {

        const std::string GET;
        const std::string DEL;
        const std::string DELIM;

        // COMMAND|XX|KEY|XX|VALUE
        std::string setCommand(const std::string& key, const std::string& value);

        // COMMAND|XX|KEY
        std::string delCommand(const std::string& key);

        std::string getFileName(int DB_IDENTIFIER);

    }

}