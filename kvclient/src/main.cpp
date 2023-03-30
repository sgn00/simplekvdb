#include "rpc/client.h"
#include <iostream>
#include <string>
#include <optional>
using std::string;

int main() {
  rpc::client c("127.0.0.1", 9095);

  string input;
  while (std::getline(std::cin, input)) {
    if (!input.empty()) {
      if (input == "set") {
          std::cout << "calling set..." << std::endl;
          auto result = c.call("set", "abc", "123").as<int>();
          std::cout << result << std::endl;
      } else if (input == "del") {
        auto result = c.call("del", "abc").as<int>();
        std::cout << result << std::endl;
      } else if (input == "get") {
        auto res = c.call("get", "abc").as<std::pair<int,std::optional<string>>>();
        std::cout << res.first << ", " << res.second.value() << std::endl;
      } else {
        std::cout << "invalid input" << std::endl;
      }

    }
  }
}