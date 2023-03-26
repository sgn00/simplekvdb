#pragma once

namespace simplekvdb {

class AOFLoader {

public:

    explicit AOFLoader(const std::string& filePath);

    void loadAndExecute(kvStore& kvStore);


};

}