#include <mutex>
#include <iostream>
#include <condition_variable>
#include <fstream>
#include <queue>
#include <thread>
#include <ctime>
#include <sstream>
#include <ivclogger.hpp>

/* Visual Concepts Logger
* Custom logger that extends IVCLogger interface
* This logger is thread safe and supports multiple sinks
* [TODO] Add support for custom formatters
* [TODO] Add support for cout style logging Ex: log << "Hi this is Anish with fav. no." << 5 << " and this is a test for visual concepts"
*/

namespace vclogger{
    class VCLogger: public IVCLogger {
    public:
        // use smart pointers to make sure to share ownership between user and logger
        // [TODO] explore unique pointers
        void addSink(std::shared_ptr<ISink> sink) {
            std::unique_lock<std::mutex> sinklock(mtxLogger_);
            sinks_.push_back(sink);
        }
        // Set log levels for all sinks
        void setLogLevels(VCLogLevel logLevel) {
            std::unique_lock<std::mutex> sinklock(mtxLogger_);
            for(auto &sink:sinks_) {
                sink->setLogLevel(logLevel);
            }
        }
        // Do we need this method?
        void setLogLevel(VCLogLevel logLevel, std::shared_ptr<ISink> sink) {
            std::unique_lock<std::mutex> sinklock(mtxLogger_);
            sink->setLogLevel(logLevel);
        }
        // log messages
        void log(VCLogLevel logLevel, const std::string& message) {
            std::unique_lock<std::mutex> sinklock(mtxLogger_);
            for(auto &sink:sinks_) {
                sink->sink(logLevel, format_log_message(logLevel, message));
            }
        }
        void log(VCLogLevel logLevel, std::string& message) {
            std::unique_lock<std::mutex> sinklock(mtxLogger_);
            for(auto &sink:sinks_) {
                sink->sink(logLevel, format_log_message(logLevel, message));
            }
        }
        VCLogger() = default;
        ~VCLogger() = default;

    private:
        std::vector<std::shared_ptr<ISink>> sinks_;
        std::mutex mtxLogger_;
        std::unique_ptr<ISink> sink_;

        // [TODO] Add support for custom formatters. Can add colors, regex filters, etc.
        // Predefined log formatting to print timestamp, log level and message
        std::string format_log_message(VCLogLevel logLevel, const std::string& message) {
            std::time_t t = std::time(nullptr);
            std::tm tm = *std::localtime(&t);
            std::string time = std::to_string(tm.tm_year + 1900) + "-" + std::to_string(tm.tm_mon + 1) + "-" + std::to_string(tm.tm_mday) + " " + std::to_string(tm.tm_hour) + ":" + std::to_string(tm.tm_min) + ":" + std::to_string(tm.tm_sec);
            return time + " " + get_enum_value(logLevel) + " "+ message;
        }
    };
}