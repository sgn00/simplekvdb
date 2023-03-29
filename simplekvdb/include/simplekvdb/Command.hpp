#pragma once
#include <variant>
#include <string>

namespace simplekvdb {

    struct SetCommand {
        std::string key;
        std::string value;
    };

    struct DelCommand {
        std::string key;
    };

    using tCommand = std::variant<simplekvdb::SetCommand,simplekvdb::DelCommand>;
}