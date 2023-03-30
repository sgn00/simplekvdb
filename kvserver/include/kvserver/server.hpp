#pragma once
#include "rpc/server.h"
#include "simplekvdb/KvStore.hpp"

namespace kvserver {


class Server {

public:

    Server();

    void start();

private:

    const int size = 10'000'000;

    const int port = 9095;

    const int DB_IDENTIFIER = 1;

    simplekvdb::KvStore kvStore;

    rpc::server server;

    void bindFunctions();

    int set(const std::string& key, const std::string& value);

    std::pair<int,std::optional<std::string>> get(const std::string& key);

    int del(const std::string& key);

};


}