#pragma once
#include <unordered_map>
#include <string>
#include <optional>

namespace simplekvdb {

    class Database {

    public:

        std::optional<std::string> Get(const std::string& key);

        bool Put(const std::string& key, const std::string& value);

    private:

        std::unordered_map<std::string,std::string> store;

    };


}

