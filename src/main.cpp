// Main entry point for the RadCat application
#include "App.hpp"

int main(int argc, char* argv[]) {
    Controller appController;
    
    // Debug
    std::cout << "Press Enter to stop the application..." << std::endl;
    std::cin.get();

    appController.stop();
    return 0;
}