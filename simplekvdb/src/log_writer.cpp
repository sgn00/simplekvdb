#include "simplekvdb/LogWriter.hpp"

#include <chrono>
#include <iterator>

using namespace simplekvdb;

LogWriter::LogWriter(const std::string &filePath) : exitFlag(false) {
  logFile = std::ofstream(filePath, std::ios_base::app);
  asyncWriterThread = std::thread(&LogWriter::asyncWriter, this);
  buffer.reserve(1024);
}

LogWriter::~LogWriter() {
  exitFlag = true;
  cv.notify_one();
  asyncWriterThread.join();
  writeToFile();
}

void LogWriter::log(const std::string &message) {
  std::unique_lock<std::mutex> lock(bufferMutex);
  buffer.insert(buffer.end(), message.begin(), message.end());
  buffer.push_back('\n');
  cv.notify_one();
}

void LogWriter::writeToFile() {
  std::ostream_iterator<char> out_it(logFile);
  std::copy(buffer.begin(), buffer.end(), out_it);
  logFile.flush();
  buffer.clear();
}

void LogWriter::asyncWriter() {
  while (!exitFlag) {
    std::unique_lock<std::mutex> lock(bufferMutex);
    cv.wait_for(lock, std::chrono::seconds(asyncIntervalSeconds),
                [this] { return exitFlag.load() || !buffer.empty(); });
    writeToFile();
  }
}
