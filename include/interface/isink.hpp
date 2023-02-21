#pragma once
/* Sink interface for Viscual Concepts Logger
This interface exposes APIs for:
* 1. Setting the sink location
* 2. Writing to the sink
* 3. Setting the log level

[TODO]
* Adding custom records
* Adding custom write polcies. Blocking, non-blocking, etc.
*/
#include <types/levels.hpp>
#include <string>

namespace vclogger {
    class ISink {
        public:
            // Write to the sink with the given log level 
            // and discards the message if the log level is lower than the sink's log level
            virtual void sink(VCLogLevel loglevel, std::string message) = 0;
            // Set the sink location for example, networl location, file location, etc.
            virtual void setSinkLocation(std::string location) = 0;
            // Set the log level for the sink
            virtual void setLogLevel(VCLogLevel logLevel) = 0;
            virtual ~ISink() = default;
    };
}

