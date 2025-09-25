import OSImportManager
import UserInterface
import atexit

# Create App thread and clean on exit
app = OSImportManager.App.App() # Start initalization and logic loop if initalization is successful
def cleanup():
    app.Stop()
atexit.register(cleanup)

# Create UI in main thread
window1 = UserInterface.CreateUserInterface1()
