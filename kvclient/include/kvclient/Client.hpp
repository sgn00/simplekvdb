#pragma once

#include <string>

#include "kvclient/Command.hpp"
#include "rpc/client.h"

namespace kvclient {

class Client {
 public:
  Client(const std::string &serverIP, int port);

  std::string send(const TParseCommand &command);

 private:
  const std::string serverIP;

  const int port;

  rpc::client client;

  static std::string getErrorMessage(int errorCode);

  static bool statusIsOk(int status);

  std::string sendSet(const SetCommand &setCommand);

  std::string sendGet(const GetCommand &getCommand);

  std::string sendDel(const DelCommand &delCommand);

  std::string sendHSet(const HSetCommand &hsetCommand);

  std::string sendHDel(const HDelCommand &hdelCommand);

  std::string sendHGet(const HGetCommand &hgetCommand);
};

class UnknownCommandException : public std::runtime_error {
 public:
  explicit UnknownCommandException(const std::string &message)
      : std::runtime_error(message) {}
};

}  // namespace kvclient