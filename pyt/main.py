import platform
print(platform.architecture())

import OSImportManager
import UserInterface
import atexit

# Create App thread and clean on exit
controller = OSImportManager.controller.Controller() # Start initalization and logic loop if initalization is successful
def cleanup():
    controller.stop()
atexit.register(cleanup)

# Create and initialize UI
mainUI = UserInterface.MainUI()

# Link UI to controller
mainUI.controllerClass = controller

mainUI.window.mainloop() # Start UI loop (Blocking)