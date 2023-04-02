#include "kvclient/client.hpp"
#include "simplekvdb/Result.hpp"

using namespace kvclient;
using namespace simplekvdb;

using CommResult = std::tuple<int,int,std::string,int>;

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

std::string Client::getErrorMessage(int errorCode) {
    switch (errorCode) {
        case static_cast<int>(simplekvdb::Result::ErrorCode::KeyNotFound):
            return "(server error) key not found";
        case static_cast<int>(simplekvdb::Result::ErrorCode::FieldNotFound):
            return "(server error) field not found";
        case static_cast<int>(simplekvdb::Result::ErrorCode::WrongType):
            return "(server error) wrong key type";
        case static_cast<int>(simplekvdb::Result::ErrorCode::DBFull):
            return "(server error) DB is full";
        case static_cast<int>(simplekvdb::Result::ErrorCode::None):
            return "(server) no error";
        default:
            return "(server) unknown error";
    }
}

std::string Client::sendSet(const SetCommand& setCommand) {
    auto [status, errorCode, strResult, intResult] = client.call(SET, setCommand.key, setCommand.value).as<CommResult>();

    if (status == static_cast<int>(simplekvdb::Result::Status::OK)) {
        return "OK";
    }

    return getErrorMessage(errorCode);
}

std::string Client::sendGet(const GetCommand& getCommand) {
    auto [status, errorCode, strResult, intResult] = client.call(GET, getCommand.key).as<CommResult>();

    if (status == static_cast<int>(simplekvdb::Result::Status::OK)) {
        return "\"" + strResult + "\"";
    }

    return getErrorMessage(errorCode);
}

std::string Client::sendDel(const DelCommand& delCommand) {
    auto [status, errorCode, strResult, intResult] = client.call(DEL, delCommand.key).as<CommResult>();

    if (status == static_cast<int>(simplekvdb::Result::Status::OK)) {
        return "(integer) " + std::to_string(intResult);
    }

    return getErrorMessage(errorCode);
}