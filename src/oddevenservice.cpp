#include <thread>
#include <config.hpp>
using namespace vclogger;
/*
* A simple service that finds all even and odd numbers from 0 to n using two threads. 1. Odd number thread 2. Even number thread
* We use our logging module to log the events in the service.
* Further, we defimed macros for easy logging
*/

// [Hack] This macro only works if the logger is the shared global pointer for IVCLogger
#define LOG_DEBUG(...) (logger->log(VCLogLevel::VCDebug, "Line: "+ std::to_string(__LINE__)+ " File: "+ __FILE__+" "+__VA_ARGS__))
#define LOG_INFO(...) (logger->log(VCLogLevel::VCInfo, "Line: "+ std::to_string(__LINE__)+ " File: "+ __FILE__+" "+__VA_ARGS__))
#define LOG_WARN(...) (logger->log(VCLogLevel::VCWarn, "Line: "+ std::to_string(__LINE__)+ " File: "+ __FILE__+" "+__VA_ARGS__))
#define LOG_ERROR(...) (logger->log(VCLogLevel::VCError, "Line: "+ std::to_string(__LINE__)+ " File: "+ __FILE__+" "+__VA_ARGS__))
#define LOG_CRITICAL(...) (logger->log(VCLogLevel::VCCritical, "Line: "+ std::to_string(__LINE__)+ " File: "+ __FILE__+" "+__VA_ARGS__))

// Global Logger
std::shared_ptr<IVCLogger> logger;


// find all even numbers
void find_all_even_numbers(int n) {
    LOG_INFO("Starting even number search");
    for(int i = 0; i < n; i++) {
        if(i % 2 == 0) {
            LOG_INFO("Even number: "+std::to_string(i));
        }
    }
    // testing exception handling logs
    try{
        throw std::invalid_argument("Throwing Invalid Args Exception");
        // LOG_INFO("Failed in throwing Exception");
    }
    catch(std::exception &e) {
        LOG_ERROR("Exception: "+std::string(e.what()));
    }
}

// find all odd numbers
void find_all_odd_numbers(int n) {
    LOG_INFO("Starting odd number search");
    for(int i = 0; i < n; i++) {
        if(i % 2 != 0) {
            LOG_INFO("Odd number: "+std::to_string(i));
        }
    }
}

int main() {
    /*
    * Setting up the logger with two sinks: Console and File.
    * Passing the logger to the child threads for seamless logging
    */

    logger = Config().getLogger();
    // one way to log
    logger->log(VCLogLevel::VCInfo, "First good log");
    
    std::thread t1(find_all_even_numbers, 100);
    std::thread t2(find_all_odd_numbers, 100);
    t1.join();
    t2.join();

    // another way to log if you have the logger as a global variable
    LOG_INFO("OddEven service completed sucessfully!");
}


