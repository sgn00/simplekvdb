#include "LogWriter.hpp"

#include <chrono>

using namespace simplekvdb;

LogWriter::LogWriter(const std::string& filePath, bool enabled)
    : enabled(enabled), exitFlag(false) {
    if (enabled) {
        logFile = std::ofstream(filePath,std::ios_base::app);
        asyncWriterThread = std::thread(&LogWriter::asyncWriter, this);
    }
}

LogWriter::~LogWriter() {
    if (enabled) {
        exitFlag = true;
        cv.notify_one();
        asyncWriterThread.join();
        writeToFile();
    }

}

void LogWriter::log(const std::string& message) {
    if (enabled) {
        std::unique_lock<std::mutex> lock(bufferMutex);
        buffer.append(message + "\n");
        cv.notify_one();
    }
}

void LogWriter::writeToFile() {
    logFile << buffer;
    logFile.flush();
    buffer.clear();
}

void LogWriter::asyncWriter() {
    while (!exitFlag) {
        std::unique_lock<std::mutex> lock(bufferMutex);
        cv.wait_for(lock, std::chrono::seconds(asyncIntervalSeconds),
                    [this] { return exitFlag.load(); });
        writeToFile();
    }
}
