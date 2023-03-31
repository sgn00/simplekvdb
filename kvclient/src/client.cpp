#include "kvclient/client.hpp"
#include "simplekvdb/RetCode.hpp"

using namespace kvclient;
using namespace simplekvdb;

Client::Client(const std::string& serverIP, int port) : serverIP(serverIP), port(port), client(serverIP, port) {

}


std::string Client::send(const tParseCommand& command) {
    if (std::holds_alternative<SetCommand>(command)) {
        return sendSet(std::get<SetCommand>(command));
    } else if (std::holds_alternative<GetCommand>(command)) {
        return sendGet(std::get<GetCommand>(command));
    } else if (std::holds_alternative<DelCommand>(command)) {
        return sendDel(std::get<DelCommand>(command));
    } else {
        // throw invalid command exception
    }
}

std::string Client::sendSet(const SetCommand& setCommand) {
    int retCode = client.call(SET, setCommand.key, setCommand.value).as<int>();
    if (retCode == static_cast<int>(RetCode::SUCCESS) || retCode == static_cast<int>(RetCode::SUCCESS_AND_EXISTED)) {
        return "OK";
    } else {
        return "(integer) 0";
    }
}

std::string Client::sendGet(const GetCommand& getCommand) {
    auto [retCode, val] = client.call(GET, getCommand.key).as<std::pair<int,std::optional<std::string>>>();
    if (retCode == static_cast<int>(RetCode::SUCCESS)) {
        return "\"" + val.value() + "\"";
    } else if (retCode == static_cast<int>(RetCode::DOES_NOT_EXIST)) {
        return "<nil>";
    } else {
        return "(integer) 0";
    }
}

std::string Client::sendDel(const DelCommand& delCommand) {
    int retCode = client.call(DEL, delCommand.key).as<int>();
    if (retCode == static_cast<int>(RetCode::SUCCESS)) {
        return "(integer) 1";
    } else {
        return "(integer) 0";
    }
}