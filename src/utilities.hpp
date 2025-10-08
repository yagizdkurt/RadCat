#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include "Debug.hpp"

namespace Utilities {

    // Hash checking and data validation function
    struct DataValidationResult {
        bool isValid;
        bool hasSuspiciousPattern;
        unsigned char checksum;
        std::string hexData;
        std::string message;
    };

    // Function declarations (implementations in utilities.cpp)
    DataValidationResult validateReceivedData(const unsigned char* data, size_t dataSize, const std::string& context = "Data");
    bool validateAndLogData(const unsigned char* data, size_t dataSize, const std::string& context = "Data");
    void sleepMs(int milliseconds);
    
    inline bool validate(const unsigned char* data, size_t dataSize) {
        auto result = validateReceivedData(data, dataSize);
        return result.isValid && !result.hasSuspiciousPattern;
    }

}