#include "kvserver/server.hpp"
#include <string>
#include <iostream>
using std::string;

int main() {
  kvserver::Server server;
  server.start();
  return 0;
}