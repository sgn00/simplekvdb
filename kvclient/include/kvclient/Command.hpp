#pragma once
#include <string>
#include <variant>

namespace kvclient {

    const std::string GET = "GET";
    const std::string SET = "SET";
    const std::string DEL = "DEL";
    const std::string QUIT = "QUIT";
    
    struct SetCommand {
        std::string key;
        std::string value;
    };

    struct DelCommand {
        std::string key;
    };

    struct GetCommand {
        std::string key;
    };

    struct QuitCommand {};

    using tParseCommand = std::variant<SetCommand,DelCommand,GetCommand,QuitCommand>;
}