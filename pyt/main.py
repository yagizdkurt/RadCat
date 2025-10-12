import platform
print(platform.architecture())

import OSImportManager
import UserInterface
import atexit

# Create App thread and clean on exit
controller = OSImportManager.controller.Controller() # Start initalization and logic loop if initalization is successful,
cpp = controller.getPyManager()  # Access PyManager instance

def cleanup():
    controller.stop()
atexit.register(cleanup)

# Create and initialize UI
mainUI = UserInterface.MainUI()

# Link UI to controller
mainUI.controllerClass = controller
mainUI.cpp = cpp
mainUI.starter() # Start loop functions

mainUI.window.mainloop() # Start UI loop (Blocking)