import sys

if sys.platform.startswith("win"):
    import YDYTU1_py as App
elif sys.platform.startswith("linux"):
    import YDYTU1_py as App
elif sys.platform.startswith("darwin"):
    import YDYTU1_py as App
else:
    raise RuntimeError("Unsupported platform!")


