#pragma once
#include <string>

#include "kvclient/Command.hpp"
#include "rpc/client.h"

namespace kvclient {

class Client {
 public:
  Client(const std::string &serverIP, int port);

  std::string send(const tParseCommand &command);

 private:
  const std::string serverIP;

  const int port;

  rpc::client client;

  static std::string getErrorMessage(int errorCode);

  static bool statusIsOK(int status);

  std::string sendSet(const SetCommand &setCommand);

  std::string sendGet(const GetCommand &getCommand);

  std::string sendDel(const DelCommand &delCommand);

  std::string sendHSet(const HSetCommand &hsetCommand);

  std::string sendHDel(const HDelCommand &hdelCommand);

  std::string sendHGet(const HGetCommand &hgetCommand);
};

}  // namespace kvclient