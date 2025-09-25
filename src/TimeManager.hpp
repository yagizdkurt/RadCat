#pragma once
#include "Includes.hpp"

struct TimedRequest {
    float remaining;
    std::function<void()> callback;
};

class TimeManager {
public:
    TimeManager() : last(Clock::now()) {}
    static TimeManager& getInstance() {
        static TimeManager instance;
        return instance;
    }
    using Clock = std::chrono::steady_clock;


    //TRACKER FUNCTIONS
    float elapsedMS() {
    auto now = Clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - last);
    last = now;
    return static_cast<float>(diff.count());
    }

    float elapsedS() {
    auto now = Clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - last);
    last = now;
    return static_cast<float>(diff.count());
    }
    
    float peekMS() const {
    auto now = Clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - last);
    return static_cast<float>(diff.count());
    }

    float peekS() const {
    auto now = Clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - last);
    return static_cast<float>(diff.count());
    }

    void resetTracker() { last = Clock::now(); }


    //REQUEST FUNCTIONS
    void checkRequests();
    void updateRequests(float delta);
    template<typename Func, typename... Args> void timeRequest(float delay, Func&& func, Args&&... args) {
        auto callback = [f = std::forward<Func>(func), ... a = std::forward<Args>(args)]() mutable { f(a...); };
        timedRequests.push_back({delay, callback});
    }
    //float delay is in milliseconds


    void timeUpdate();

private:
    Clock::time_point last;
    std::vector<TimedRequest> timedRequests;

};

inline TimeManager& TimeM = TimeManager::getInstance();