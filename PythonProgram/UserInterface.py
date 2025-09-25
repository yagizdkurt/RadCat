import tkinter as tk

InitialWindowWidth = 800
InitialWindowHeight = 600

def CreateUserInterface1():
    window1 = tk.Tk()

    window1.title("YTU test 1")
    window1.geometry(f"{InitialWindowWidth}x{InitialWindowHeight}")

    label = tk.Label(window1, text="Hello!")
    label.pack()
    button = tk.Button(window1, text="Click Me", command=lambda: print("Button clicked!"))
    button.pack()

    window1.mainloop()

    return window1
