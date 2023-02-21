#include <isink.hpp>
#include <queue>
#include <thread>
#include <mutex>
#include <iostream>
#include <condition_variable>
#include <string>
#include <iostream>
/*
* ConsoleSink class that implements ISink interface
* This class is thread safe and does non blocking logging. We do this by decoupling file writing from the main thread
* and our logic avoids busy waiting incase of low log rate.
* This kind of logging is especially useful for applications that has slow I/O or involves network storage.
*
* [Logic:] Instead of logging the message to console immediately, we will push it to a queue 
* and lazily write it to the file in a separate thread. This will help us to avoid blocking and increase performance.
* [Details:] Writer thread swaps messagequeue with writermsgqueue and flushes the logs to the console in its own time.
*/
namespace vclogger {
    class ConsoleSink: public ISink {
    public:
        // inherited from ISink
        void sink(VCLogLevel mLogLevel, const std::string& message) {
            if(mLogLevel < vcLogLevel_) return;
            std::unique_lock<std::mutex> msglock(mtxMsgQueue_);
            msgQueue_.push(std::string(message));
            msglock.unlock();
            cv_.notify_one();
        }
        // console location is always console, can be extended to support other GUI options but I don't know about them
        void setSinkLocation(const std::string& location) {
            std::cout << "[ConsoleSink] Sink location set to console:" << location <<std::endl;
        }
        void setSinkLocation(const char* location) {
            std::cout << "[ConsoleSink] Sink location set to console:" << location <<std::endl;
        }
        // sets the log level for the sink
        void setLogLevel(VCLogLevel mLogLevel) {
            std::unique_lock<std::mutex> sinklock(mtxConsoleSink_);
            vcLogLevel_ = mLogLevel;
            std::cout << "[ConsoleSink] Sink log level set to " << get_enum_value(vcLogLevel_) << std::endl;
        }
        // constructor to initialize the writer thread
        ConsoleSink() {
            runCondition_ = true;
            vcLogLevel_ = VCLogLevel::VCInfo;
            writerThread_ = std::thread(&ConsoleSink::lazy_writer, this);
        }
        // destructor to ensure that the writer thread logs pending messages and is joined before exiting
        ~ConsoleSink() {
            runCondition_ = false;
            cv_.notify_all();
            writerThread_.join();
            std::cout<<"[ConsoleSink] ConsoleSink destructor called exiting safely"<<std::endl;
        }

    private:
        VCLogLevel vcLogLevel_;
        std::string sinkLocation_;
        bool runCondition_;
        std::thread writerThread_;
        std::mutex mtxMsgQueue_;
        std::mutex mtxConsoleSink_;
        std::condition_variable cv_;
        std::queue<std::string> msgQueue_;
        // [Logic:] Instead of logging the message to console immediately, we will push it to a queue
        // and lazily write it to the file in a separate thread. This will help us to avoid blocking and increase performance.
        // [Details:] Writer thread swaps messagequeue with writermsgqueue and flushes the logs to the console in its own time.
        void lazy_writer() {
            std::queue<std::string> writermsgqueue;
            while(runCondition_ || !msgQueue_.empty()) {
                std::unique_lock<std::mutex> msglock(mtxMsgQueue_);
                cv_.wait(msglock, [this]{return !msgQueue_.empty() || !runCondition_;});
                writermsgqueue.swap(msgQueue_);
                msglock.unlock();
                cv_.notify_one();
                while(!writermsgqueue.empty()) {
                    std::cout << writermsgqueue.front() << std::endl;
                    writermsgqueue.pop();
                }
            }
        }
    };
}