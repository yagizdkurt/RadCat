import sys

if sys.platform.startswith("win"):
    print("You are using Windows")
    import YDYTU1_py as controller
elif sys.platform.startswith("linux"):
    print("You are using Linux")
    import YDYTU1_py as controller
elif sys.platform.startswith("darwin"):
    print("You are using MacOS")
    import YDYTU1_py as controller
else:
    raise RuntimeError("Unsupported platform!")


