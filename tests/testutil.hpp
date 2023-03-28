#pragma once

#include <filesystem>
#include "LoggingUtil.hpp"

using namespace simplekvdb;

class DeleteFile {
public:

    int IDENT;

    DeleteFile(int ident) : IDENT(ident) {}

    ~DeleteFile() {
        std::filesystem::path filePath = aoflogging::getFileName(IDENT);
        std::filesystem::remove(filePath);
    }
};