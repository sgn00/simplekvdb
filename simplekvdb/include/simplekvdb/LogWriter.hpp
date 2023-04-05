#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <atomic>

namespace simplekvdb {

class LogWriter {

public:

    explicit LogWriter(const std::string& filePath);
    
    ~LogWriter();

    void log(const std::string& message);

private:

    void writeToFile();

    void asyncWriter();

    std::ofstream logFile;
    std::vector<char> buffer;
    std::mutex bufferMutex;
    std::condition_variable cv;
    std::atomic<bool> exitFlag;
    std::thread asyncWriterThread;
    unsigned int asyncIntervalSeconds = 10;

};



};