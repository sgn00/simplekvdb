#pragma once
#include <string>
#include <vector>

namespace simplekvdb {

    namespace aoflogging {

        const std::string SET = "SET";
        const std::string DEL = "DEL";
        const std::string HSET = "HSET";
        const std::string HDEL = "HDEL";
        const std::string DELIM = "|";

        // SET|XX|KEY|XX|VALUE
        std::string stringifySetCommand(const std::string& key, const std::string& value);

        // DEL|XX|KEY
        std::string stringifyDelCommand(const std::string& key);

        // HSET|XX|KEY|XX|FIELD1|XX|VALUE1|XX|FIELD2|XX|VALUE2
        std::string stringifyHSetCommand(const std::string& key, const std::vector<std::pair<std::string,std::string>>& fieldValuePairs);

        // HDEL|XX|KEY|XX|FIELD1|XX|FIELD2
        std::string stringifyHDelCommand(const std::string& key, const std::vector<std::string>& fields);

        std::string getFileName(int DB_IDENTIFIER);


    }

}