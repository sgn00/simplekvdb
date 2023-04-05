#include <fmt/core.h>

#include <iostream>
#include <optional>
#include <string>

#include "kvclient/CommandParser.hpp"
#include "kvclient/client.hpp"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fmt::print("Wrong num of args\n");
    fmt::print(stderr, "Please specify server IP and server port number.\n");
    fmt::print(stderr,
               "Format: ./kvclient <server ip> <server port> eg. ./kvserver "
               "127.0.0.1 8080\n");
    return 1;
  }

  // No input checking
  std::string ip(argv[1]);
  std::string portStr(argv[2]);
  int port(std::stoi(portStr));

  kvclient::Client client(ip, port);

  fmt::print("Connected to {}:{}\n", ip, port);

  std::string input;
  fmt::print("> ");
  while (std::getline(std::cin, input)) {
    try {
      kvclient::tParseCommand command =
          kvclient::CommandParser::parseLine(input);
      if (std::holds_alternative<kvclient::QuitCommand>(command)) {
        fmt::print("OK\n");
        break;
      }
      std::string res = client.send(command);
      fmt::print("{}\n", res);
    } catch (const kvclient::InvalidCommandException &e) {
      fmt::print(stderr, "(error) {}\n", e.what());
    }
    fmt::print("> ");
  }
}