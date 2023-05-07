#include <condition_variable>
#include <fstream>
#include <iostream>
#include <isink.hpp>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#define MESSAGE_BATCH 100
#define DEFAULT_FLUSH_INTERVAL_IN_MS 500
#define DEFAULT_MAX_BUFFER_SIZE 100

namespace vclogger {
class FileSink : public ISink {
public:
  FileSink() : FileSink("vclogger.log") {}

  FileSink(const char *location)
      : log_level_(VCLogLevel::VCInfo),
        flush_interval_in_ms_(DEFAULT_FLUSH_INTERVAL_IN_MS),
        max_buffer_size_(DEFAULT_MAX_BUFFER_SIZE), is_running_(true) {
    setSinkLocation(location);
    flush_thread_ = std::thread(&FileSink::flushLoop, this);
  }

  FileSink(int flush_interval_in_ms, int max_buffer_size)
      : flush_interval_in_ms_(flush_interval_in_ms),
        max_buffer_size_(max_buffer_size), is_running_(true) {}

  ~FileSink() {
    {
      std::lock_guard<std::mutex> lock(mutex_);
      is_running_ = false;
      flush_cv_.notify_one();
    }
    flush_thread_.join();
  }

  void setSinkLocation(const char *dst) {
    std::lock_guard<std::mutex> lock(file_mutex_);
    if (log_file_stream_.is_open()) {
      log_file_stream_.close();
    }
    log_file_stream_.open(dst);
  }
  void sink(VCLogLevel level, const std::string &message) {
    if (level < this->log_level_)
      return;
    std::lock_guard<std::mutex> lock(mutex_);
    log_queue_.push(message);
    if (log_queue_.size() >= max_buffer_size_) {
      flush_cv_.notify_one();
    }
  }

  void setLogLevel(VCLogLevel level) {
    std::lock_guard<std::mutex> lock(mutex_);
    log_level_ = level;
  }

private:
  void flushLoop() {
    while (true) {
      std::unique_lock<std::mutex> lock(mutex_);
      if (!is_running_ && !log_queue_.empty()) {
        return;
      }
      if (log_queue_.empty()) {
        flush_cv_.wait(lock);
        continue;
      }
      auto now = std::chrono::steady_clock::now();
      if (now - last_flush_time_ >=
              std::chrono::milliseconds(flush_interval_in_ms_) ||
          log_queue_.size() >= max_buffer_size_) {

        std::queue<std::string> messages_to_flush;
        messages_to_flush.swap(log_queue_);
        lock.unlock();
        flushMessages(messages_to_flush);
        lock.lock();
        last_flush_time_ = std::chrono::steady_clock::now();
      } else {
        flush_cv_.wait_for(lock,
                           std::chrono::milliseconds(flush_interval_in_ms_));
      }
    }
  }

  void flushMessages(std::queue<std::string> messages) {
    std::lock_guard<std::mutex> lock(file_mutex_);
    while (!messages.empty()) {
      log_file_stream_ << messages.front() << "\n";
      messages.pop();
    }
  }

  const int flush_interval_in_ms_;
  const int max_buffer_size_;

private:
  VCLogLevel log_level_;
  std::ofstream log_file_stream_;
  std::queue<std::string> log_queue_;
  std::chrono::steady_clock::time_point last_flush_time_;
  std::mutex mutex_;
  std::mutex file_mutex_;
  std::condition_variable flush_cv_;
  std::thread flush_thread_;
  bool is_running_;
};
}; // namespace vclogger