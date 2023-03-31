#include <iostream>
#include <string>
#include <optional>
#include "kvclient/client.hpp"
#include "kvclient/CommandParser.hpp"


int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "Wrong num of args" << std::endl;
    std::cerr << "Please specify server IP and server port number." << std::endl;
    std::cerr << "Format: ./kvclient <server ip> <server port> eg. ./kvserver 127.0.0.1 8080" << std::endl;
    return 1;
  }

  // No input checking
  std::string ip(argv[1]);
  std::string portStr(argv[2]);
  int port(std::stoi(portStr));


 // rpc::client c(ip, port);
  kvclient::Client client(ip, port);

  std::cout << "Connected to " << ip << ":" << port << std::endl;

  std::string input;
  std::cout << "> ";
  while (std::getline(std::cin, input)) {
    try {
      kvclient::tParseCommand command = kvclient::CommandParser::parseLine(input);
      if (std::holds_alternative<kvclient::QuitCommand>(command)) {
        std::cout << "OK" << std::endl;
        break;
      }
      std::string res = client.send(command);
      std::cout << res << std::endl;
    } catch (const kvclient::InvalidCommandException& e) {
      std::cerr << "(error) " << e.what() << std::endl;
    }
    std::cout << "> ";
  }
  // while (std::getline(std::cin, input)) {
  //   if (!input.empty()) {
  //     if (input == "set") {
  //         std::cout << "calling set..." << std::endl;
  //         auto result = c.call("set", "abc", "123").as<int>();
  //         std::cout << result << std::endl;
  //     } else if (input == "del") {
  //       auto result = c.call("del", "abc").as<int>();
  //       std::cout << result << std::endl;
  //     } else if (input == "get") {
  //       auto res = c.call("get", "abc").as<std::pair<int,std::optional<string>>>();
  //       std::cout << res.first << ", " << res.second.value() << std::endl;
  //     } else {
  //       std::cout << "invalid input" << std::endl;
  //     }

  //   }
  // }
}