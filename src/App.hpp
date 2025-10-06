#include "Includes.hpp"

class Controller{
private:

    std::mutex dataMutex;
    std::thread LogicThread;
    std::atomic<bool> isRunning{false};

public:

    //Modules
    DIOHandler dataHandler;

    Controller() { if(systemInitializor()) run(); }

    //Main Methods
    void run();
    void logic();
    void stop();
    bool systemInitializor();


    // PYTHON INTERFACE METHODS
    void connectMiniX();
    void disconnectMiniX();
    void testButton();

};