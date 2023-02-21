#pragma once
#include <string>
/*
* Defines the log levels for the logger and gives a static utility function to get the string value of the enum
*/
namespace vclogger {
    // Supports 5 log levels: VCDebug, VCInfo, VCWarn, VCError, VCCritical
    enum class VCLogLevel{
        VCDebug, VCInfo, VCWarn, VCError, VCCritical
    };
    static std::string get_enum_value(VCLogLevel mloglevel) {
        switch(mloglevel) {
            case VCLogLevel::VCDebug:
                return "VCDebug";
            case VCLogLevel::VCInfo:
                return "VCInfo";
            case VCLogLevel::VCWarn:
                return "VCWarn";
            case VCLogLevel::VCError:
                return "VCError";
            case VCLogLevel::VCCritical:
                return "VCCritical";
            default:
                return "VCDebug";
        }
    }
}