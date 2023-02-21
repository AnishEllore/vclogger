#pragma once

#include <vclogger.hpp>
#include <consolesink.hpp>
#include <filesink.hpp>
/*
* Config allows us to set custom sinks and custom logges without modifying the application logic aka main.cpp
* Uses Dependency Injection pattern
* [TODO] Can load configsettings from disk for service specific logging
*/

namespace vclogger {
    
    class Config {
        public:  
            std::shared_ptr<IVCLogger> getLogger() {
                return logger_;
            }
            // Setting up the logger with logger: VCLogger and two sinks: Console and File.
            Config() {
                std::shared_ptr<ISink> csink = std::make_shared<ConsoleSink>();
                std::shared_ptr<ISink> fsink = std::make_shared<FileSink>();
                //fsink->setSinkLocation("oddevenservice.log");
                sinks_.push_back(fsink);
                sinks_.push_back(csink);
                logger_ = std::make_shared<VCLogger>();
                for(auto &sink:sinks_) {
                    logger_->addSink(sink);
                }
            }
            ~Config() = default;
        private:
            std::shared_ptr<IVCLogger> logger_;
            std::vector<std::shared_ptr<ISink>> sinks_;
    };
}