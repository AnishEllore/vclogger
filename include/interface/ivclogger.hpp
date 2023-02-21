#pragma once
/* Logger interface for Viscual Concepts
This interface exposes APIs for:
* 1. Logging messages
* 2. Setting multiple custom sinks
* 3. Setting log levels for all sinks
* 4. Setting log levels for individual sinks
* [TODO]
* 1. Adding custom records
* 2. Adding custom formatters
* 3. Adding custom sinks
* 4. Add support for variadic templates
*/ 

#include <string>
#include <types/levels.hpp>
#include <isink.hpp>
namespace vclogger {
    class IVCLogger
    {
    public:
        // generic interface to log messages
        virtual void log(VCLogLevel mpriority, std::string message) = 0;
        // interface to change log levels at run time depemnding on the system load
        virtual void setLogLevels(VCLogLevel logLevel) = 0;
        // interface to add custom sinks
        virtual void addSink(std::shared_ptr<ISink> sink) = 0;
        virtual ~IVCLogger() = default;

    };
}
