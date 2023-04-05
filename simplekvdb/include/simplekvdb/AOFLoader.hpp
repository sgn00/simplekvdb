#pragma once

#include <string>

#include "Command.hpp"
#include "KvStore.hpp"

namespace simplekvdb {

namespace aoflogging {

class AOFLoader {
 public:
  static bool loadAndExecute(KvStore& kvStore);

 private:
  static void execute(KvStore& kvStore, tCommand command);
};

}  // namespace aoflogging

}  // namespace simplekvdb