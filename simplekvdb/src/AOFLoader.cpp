#include "AOFLoader.hpp"
#include <fstream>

using namespace simplekvdb::aoflogging;


AOFLoader::AOFLoader(const std::string& filePath) : filePath(filePath) {

}

bool AOFLoader::loadAndExecute(KvStore& kvStore) {
    std::ifstream inputFile(filePath);

    // Check if the file is successfully opened
    if (!inputFile.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        // Process the line

    }

    // Close the file
    inputFile.close();
    return true;
}
