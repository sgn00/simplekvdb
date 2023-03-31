#include "kvserver/server.hpp"
#include <string>
#include <iostream>
using std::string;

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "Wrong num of args" << std::endl;
    std::cerr << "Please specify port number and whether logging is enabled:" << std::endl;
    std::cerr << "Format: ./kvserver <port> <true|false> eg. ./kvserver 8080 true" << std::endl;
    return 1;
  }

  char* firstArg = argv[1];
  std::string portStr(firstArg);
  int portNum = 0;
  try {
    portNum = std::stoi(portStr);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  char* secondArg = argv[2];
  bool loggingEnabled = false;
  std::string loggingEnabledStr(secondArg);
  std::transform(loggingEnabledStr.begin(), loggingEnabledStr.end(), loggingEnabledStr.begin(),
    [](unsigned char c){ return std::tolower(c); });

  if (loggingEnabledStr == "true") {
    loggingEnabled = true;
  } else if (loggingEnabledStr == "false") {
    loggingEnabled = false;
  } else {
    std::cerr << "Please specify port number and whether logging is enabled:" << std::endl;
    std::cerr << "Format: ./kvserver <port> <true|false> eg. ./kvserver 8080 true" << std::endl;
    return 1;
  }

  kvserver::Server server(portNum, loggingEnabled);
  server.start();
  return 0;
}