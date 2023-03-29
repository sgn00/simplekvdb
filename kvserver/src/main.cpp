#include "rpc/server.h"
#include <string>
#include <iostream>
using std::string;

int main() {
  rpc::server srv(8080);

  srv.bind("echo", [](string const& s) {
    std::cout << s << std::endl;
    return string("> ") + s;
  });

  srv.run();
  return 0;
}