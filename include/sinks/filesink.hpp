#include <isink.hpp>
#include <queue>
#include <thread>
#include <mutex>
#include <iostream>
#include <condition_variable>
#include <string>
#include <iostream>
#include <fstream>
/*
* Filesink class that implements ISink interface
* This class is thread safe and does synchronous blocking logging, i.e. it will block the user until the message is flushed to the file.
* Synchronous logging is critical to applications that need accurate logs in case of crashes.
* 
*[TODO] 
* 1. File rotations
* 2. Can increase the speed by exporing different flushing techniques 
*/
namespace vclogger {
    class FileSink: public ISink {
    public:
        // inherited from ISink
        void sink(VCLogLevel mLogLevel, std::string message) {
            if(mLogLevel < vcLogLevel_) return;
            std::unique_lock<std::mutex> msglock(mtxFileSink_);
            logfile_ << message << std::endl;
        }
        // sets the sink location for the sink in a safe manner
        void setSinkLocation(std::string location) {
            std::unique_lock<std::mutex> sinklock(mtxFileSink_);
            if(logfile_.is_open()) {
                logfile_.close();
            }
            logfile_.open(location);
            logfile_ << "[FileSink] Sink location set to file " << location << std::endl;
        }
        // sets the log level for the sink
        void setLogLevel(VCLogLevel mLogLevel) {
            std::unique_lock<std::mutex> sinklock(mtxFileSink_);
            vcLogLevel_ = mLogLevel;
            logfile_ << "[ConsoleSink] Sink log level set to " << get_enum_value(vcLogLevel_) << std::endl;
        }
        // constructor to setup the sink
        FileSink() {
            vcLogLevel_ = VCLogLevel::VCInfo;
            setSinkLocation("vclogger.log");
        }
        // destructor to ensure file is closed before exiting
        ~FileSink() {
            logfile_<<"[FileSink] FileSink destructor called exiting safely"<<std::endl;
            std::unique_lock<std::mutex> sinklock(mtxFileSink_);
            if(logfile_.is_open()) {
                logfile_.close();
            }
        }

    private:
        VCLogLevel vcLogLevel_;
        std::ofstream logfile_;
        std::string sinkLocation_;
        std::mutex mtxFileSink_;
    };
}