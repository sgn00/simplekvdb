#pragma once
#include "rpc/client.h"
#include "kvclient/Command.hpp"
#include <string>

namespace kvclient {


class Client {


public:

    Client(const std::string& serverIP, int port);

    std::string send(const tParseCommand& command);


private:

    const std::string serverIP;

    const int port;

    rpc::client client;

    static std::string getErrorMessage(int errorCode);

    std::string sendSet(const SetCommand& setCommand);

    std::string sendGet(const GetCommand& getCommand);

    std::string sendDel(const DelCommand& delCommand);

};


}