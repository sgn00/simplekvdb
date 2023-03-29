#pragma once

#include <filesystem>
#include "simplekvdb/LoggingUtil.hpp"
#include <iostream>

using namespace simplekvdb;

class DeleteFile {
public:

    int IDENT;

    DeleteFile(int ident) : IDENT(ident) {}

    ~DeleteFile() {
        std::filesystem::path filePath = aoflogging::getFileName(IDENT);
        bool res = std::filesystem::remove(filePath);
    }
};