#pragma once
#include <atomic>
#include <condition_variable>
#include <fstream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

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

};  // namespace simplekvdb