#pragma once
#include <variant>
#include <string>
#include <vector>

namespace simplekvdb {

    struct SetCommand {
        std::string key;
        std::string value;
    };

    struct DelCommand {
        std::vector<std::string> keys;
    };

    struct HSetCommand {
        std::string key;
        std::vector<std::pair<std::string,std::string>> fieldValuePairs;
    };

    struct HDelCommand {
        std::string key;
        std::vector<std::string> fields;
    };

    using tCommand = std::variant<simplekvdb::SetCommand,simplekvdb::DelCommand,simplekvdb::HSetCommand,simplekvdb::HDelCommand>;
}