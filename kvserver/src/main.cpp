#include <fmt/core.h>

#include <string>

#include "kvserver/server.hpp"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fmt::print("Wrong num of args\n");
    fmt::print(stderr,
               "Please specify port number and whether logging is enabled:\n");
    fmt::print(
        stderr,
        "Format: ./kvserver <port> <true|false> eg. ./kvserver 8080 true\n");
    return 1;
  }

  char *firstArg = argv[1];
  std::string portStr(firstArg);
  int portNum = 0;

  try {
    portNum = std::stoi(portStr);
  } catch (const std::exception &e) {
    fmt::print(stderr, "{}\n", e.what());
    return 1;
  }

  char *secondArg = argv[2];
  bool loggingEnabled = false;
  std::string loggingEnabledStr(secondArg);
  std::transform(loggingEnabledStr.begin(), loggingEnabledStr.end(),
                 loggingEnabledStr.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  if (loggingEnabledStr == "true") {
    loggingEnabled = true;
  } else if (loggingEnabledStr == "false") {
    loggingEnabled = false;
  } else {
    fmt::print(stderr,
               "Please specify port number and whether logging is enabled:\n");
    fmt::print(
        stderr,
        "Format: ./kvserver <port> <true|false> eg. ./kvserver 8080 true\n");
    return 1;
  }

  kvserver::Server server(portNum, loggingEnabled);
  server.start();
  return 0;
}