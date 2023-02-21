# include <vclogger.hpp>
#include <thread>
#include <consolesink.hpp>
#include <filesink.hpp>
using namespace vclogger;

/*
* Testing different log levels
* Testing different sinks
* Testing different loggers
* Testing different loggers with multiple sinks
* Testing different loggers with multiple sinks and multiple user threads
* Testing helper macros
*[TODO]
* Unit testing catch2?
*/ 

// [Hack] This macro only works if the logger is the shared pointer for IVCLogger
#define LOG_DEBUG(...) (logger->log(VCLogLevel::VCDebug, "Line: "+ std::to_string(__LINE__)+ " File: "+ __FILE__+" "+__VA_ARGS__))
#define LOG_INFO(...) (logger->log(VCLogLevel::VCInfo, "Line: "+ std::to_string(__LINE__)+ " File: "+ __FILE__+" "+__VA_ARGS__))
#define LOG_WARN(...) (logger->log(VCLogLevel::VCWarn, "Line: "+ std::to_string(__LINE__)+ " File: "+ __FILE__+" "+__VA_ARGS__))
#define LOG_ERROR(...) (logger->log(VCLogLevel::VCError, "Line: "+ std::to_string(__LINE__)+ " File: "+ __FILE__+" "+__VA_ARGS__))
#define LOG_CRITICAL(...) (logger->log(VCLogLevel::VCCritical, "Line: "+ std::to_string(__LINE__)+ " File: "+ __FILE__+" "+__VA_ARGS__))

// Global Logger
std::shared_ptr<IVCLogger> logger;
void test_logger() {
    for(int i = 0; i < 100; i++) {
        std::string str = "Hello World " + std::to_string(i);
        logger->log(VCLogLevel::VCInfo, str);
        logger->log(VCLogLevel::VCDebug, "HelloWorld");
        logger->log(VCLogLevel::VCCritical, "Hello World ");
    }
    LOG_CRITICAL("Testing critical logs");
    LOG_INFO("Testing Info logs");
}


int main() {

    std::shared_ptr<ISink> csink = std::make_shared<ConsoleSink>();
    std::shared_ptr<ISink> fsink = std::make_shared<FileSink>();

    logger = std::make_shared<VCLogger>();
    logger->addSink(csink);
    logger->addSink(fsink);

    logger->log(VCLogLevel::VCCritical, "Hello World from main");
    LOG_CRITICAL("Testing critical logs");

    std::thread t1(test_logger);
    std::thread t2(test_logger);
    t1.join();
    t2.join();
}

