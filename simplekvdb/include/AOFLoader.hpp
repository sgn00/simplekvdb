#pragma once

#include <string>

#include "KvStore.hpp"

namespace simplekvdb {

    namespace aoflogging {
        class AOFLoader {

        public:

            explicit AOFLoader(const std::string& filePath);

            bool loadAndExecute(KvStore& kvStore);

        private:

            const std::string filePath;


        };
    }

}