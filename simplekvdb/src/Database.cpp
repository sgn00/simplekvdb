#include "Database.hpp"

using namespace simplekvdb;

std::optional<std::string> Database::Get(const std::string& key) {
    if (store.count(key)) {
        return store[key];
    } else {
        return std::nullopt;
    }
}

bool Database::Put(const std::string& key, const std::string& value) {
    bool ret = false;
    if (store.count(key)) {
        ret = true;
    }
    store[key] = value;
    return ret;
}