#include "kvserver/server.hpp"
#include "simplekvdb/AOFLoader.hpp"
#include <iostream>

using namespace kvserver;

Server::Server(int port, bool loggingEnabled) : port(port), kvStore(DB_IDENTIFIER, size, loggingEnabled), server(port) {
    std::cout << "Using port: " << port << std::endl;
    std::cout << "Logging enabled: " << (loggingEnabled ? "true" : "false") << std::endl;
}

void Server::start() {
    bindFunctions();
    std::cout << "Running server..." << std::endl;
    server.run();
}

void Server::bindFunctions() {
    server.bind("SET", 
        [this](const std::string& key, const std::string& value){ return set(key, value); }
    );

    server.bind("GET", 
        [this](const std::string& key){ return get(key); }
    ); 

    server.bind("DEL", 
        [this](const std::string& key){ return del(key); }
    );   
}

CommResult Server::set(const std::string& key, const std::string& value) {
    std::cout << "Set called: " << key << "," << value << std::endl; 
    auto res = kvStore.set(key, value);
    return convertToCommResult(res);
}

CommResult Server::get(const std::string& key) {
    std::cout << "Get called: " << key << std::endl;
    auto res = kvStore.get(key);
    return convertToCommResult(res);
}

CommResult Server::del(const std::string& key) {
    std::cout << "Del called: " << key << std::endl;
    auto res = kvStore.del(key);
    return convertToCommResult(res);
}

CommResult Server::convertToCommResult(const simplekvdb::Result& r) {
    int field1, field2, field4 = -1;
    std::string field3;
    
    field1 = static_cast<int>(r.status());
    if (r.status() == simplekvdb::Result::Status::Error) {
        field2 = static_cast<int>(r.error_code());
    } else {
        if (r.holds_type<std::string>()) {
            field3 = r.value<std::string>();
        } else if (r.holds_type<int>()) {
            field4 = r.value<int>();
        }
    }
    return std::make_tuple(field1, field2, field3, field4);
}