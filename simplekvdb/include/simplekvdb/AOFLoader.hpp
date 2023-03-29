#pragma once

#include <string>

#include "KvStore.hpp"
#include "Command.hpp"

namespace simplekvdb {

    namespace aoflogging {

            class AOFLoader {

            public:
                
                static bool loadAndExecute(KvStore& kvStore);

            private:

                static void execute(KvStore& kvStore, tCommand command);

            };

    }

}