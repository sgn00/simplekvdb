#pragma once

#include <string>

#include "Command.hpp"
#include "KvStore.hpp"

namespace simplekvdb {

namespace aoflogging {

class AofLoader {
 public:
  static bool loadAndExecute(KvStore& kvStore);

 private:
  static void execute(KvStore& kvStore, TCommand command);
};

}  // namespace aoflogging

}  // namespace simplekvdb