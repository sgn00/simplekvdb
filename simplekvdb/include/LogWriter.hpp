#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>

namespace simplekvdb {

class LogWriter {

public:

    LogWriter(const std::string& filePath, bool enabled);
    
    ~LogWriter();

    void log(const std::string& message);

private:

    void writeToFile();

    void asyncWriter();

    bool enabled;

    std::ofstream logFile;
    std::string buffer;
    std::mutex bufferMutex;
    std::condition_variable cv;
    std::atomic<bool> exitFlag;
    std::thread asyncWriterThread;
    unsigned int asyncIntervalSeconds = 60;

};



};