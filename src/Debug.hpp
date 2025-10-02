#pragma once
#include "Includes.hpp"

struct DebugClass {

    static DebugClass& getInstance() {
        static DebugClass instance;
        return instance;
    }

    // ANSI color codes
    enum class Color { DEFAULT, RED, YELLOW, GREEN, CYAN, WHITE };

    inline static std::string colorCode(Color c) {
        switch (c) {
            case Color::RED:    return "\033[31m"; // Error
            case Color::YELLOW: return "\033[33m"; // Warning
            case Color::GREEN:  return "\033[32m"; // Timestamp
            case Color::CYAN:   return "\033[36m"; // For later use
            case Color::WHITE:  return "\033[37m"; // Log
            default:            return "\033[0m";
        }
    }

    // Get current time as HH:MM:SS
    inline static std::string currentTime() {
        auto now = std::chrono::system_clock::now();
        auto timeT = std::chrono::system_clock::to_time_t(now);
        std::tm tmBuf;
    #if defined(_WIN32) || defined(_WIN64)
        localtime_s(&tmBuf, &timeT);
    #else
        localtime_r(&timeT, &tmBuf);
    #endif
        std::ostringstream oss;
        oss << std::put_time(&tmBuf, "%H:%M:%S");
        return oss.str();
    }

    // Printing
    inline static void print(const std::string& prefix, const std::string& msg, Color c, bool isError=false, int LinesBeforeMessage=1) {
        for (int i = 0; i < LinesBeforeMessage; ++i) std::cout << std::endl; // Empty Lines
        std::ostream& out = isError ? std::cerr : std::cout;
        out << colorCode(Color::GREEN) << "[" << currentTime() << "] " << colorCode(Color::DEFAULT); // Timestamp
        out << colorCode(c) << prefix << " " << msg << colorCode(Color::DEFAULT) << std::endl; // Prefix and message
    }

    // Logging functions
    inline static void Log(const std::string& msg, int LinesBeforeMessage=1)    { print("[DEBUG]:", msg, Color::WHITE, false, LinesBeforeMessage); }
    inline static void Warn(const std::string& msg, int LinesBeforeMessage=1)   { print("[WARNING]:", msg, Color::YELLOW, false, LinesBeforeMessage); }
    inline static void Error(const std::string& msg, int LinesBeforeMessage=1)  { print("[ERROR]:", msg, Color::RED, true, LinesBeforeMessage); }

    //Templated function for any type
    template<typename T>
    inline static void Log(const std::string& prefix, const T& msg, int LinesBeforeMessage=1) {
        std::ostringstream oss;
        oss << prefix << " " << msg;
        Log(oss.str(), LinesBeforeMessage);
    }

    template<typename T>
    inline static void Warn(const std::string& prefix, const T& msg, int LinesBeforeMessage=1) {
        std::ostringstream oss;
        oss << prefix << " " << msg;
        Warn(oss.str(), LinesBeforeMessage);
    }

    template<typename T>
    inline static void Error(const std::string& prefix, const T& msg, int LinesBeforeMessage=1) {
        std::ostringstream oss;
        oss << prefix << " " << msg;
        Error(oss.str(), LinesBeforeMessage);
    }

};

inline DebugClass& Debug = DebugClass::getInstance();