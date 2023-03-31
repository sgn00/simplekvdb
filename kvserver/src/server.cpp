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

int Server::set(const std::string& key, const std::string& value) {
    std::cout << "Set called: " << key << "," << value << std::endl; 
    simplekvdb::RetCode res = kvStore.set(key, value);
    return static_cast<int>(res);
}

std::pair<int,std::optional<std::string>> Server::get(const std::string& key) {
    std::cout << "Get called: " << key << std::endl;
    auto [retCode, res] = kvStore.get(key);
    return std::make_pair(static_cast<int>(retCode), res);
}

int Server::del(const std::string& key) {
    std::cout << "Del called: " << key << std::endl;
    simplekvdb::RetCode res = kvStore.del(key);
    return static_cast<int>(res);
}