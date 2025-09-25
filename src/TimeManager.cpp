#include "TimeManager.hpp"

void TimeManager::timeUpdate() {
    float deltaMS = elapsedMS();
    float deltaS = deltaMS / 1000.0f;

    updateRequests(deltaMS);
    checkRequests();
}


// REQUEST HANDLING
void TimeManager::checkRequests() {
    for (auto it = timedRequests.begin(); it != timedRequests.end(); ) {
        if (it->remaining <= 0) {
            it->callback();
            it = timedRequests.erase(it);  // Remove completed request
        } else {
            ++it;
        }
    }
}

void TimeManager::updateRequests(float delta) { for (auto& req : timedRequests) { req.remaining -= delta; } }
