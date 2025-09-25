#include "Includes.hpp"

class App{
private:

    std::mutex dataMutex;
    std::thread LogicThread;
    std::atomic<bool> isRunning{false};

public:

    App() { if(systemInitializor()) run(); }

    //Main Methods
    void run();
    void logic();
    void stop();
    bool systemInitializor();

};