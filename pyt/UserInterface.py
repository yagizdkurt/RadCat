import tkinter as tk
from tkinter import ttk


class MainUI:

# region: ----Global Variables and main starter ----

    def defineVariables(self):
        self.debug = False
        self.controllerClass = None
        self.tryingToConnectMinix = False
        self.connectedToMinix = False

    def starter(self):
        if self.initFinished:
            self.TryFetchingDataMiniX()
        else:
            self.window.after(100, self.starter)

# endregion

    def __init__(self):
        self.initFinished = False
        self.defineVariables()
        self.setupWindow()
        self.setStyle()
        self.setupMainFrames()

        self.setupTopMenu()
        self.setupTopGraph()
        self.setupRightFrame()
        self.setupBottomFrame()

        self.eventBinds()
        self.initFinished = True

# region: styling
    def setStyle(self):
        self.style = ttk.Style()
        self.style.theme_use("default")

        # Background Colors
        self.graphbgColor = "gray"
        self.attributesbgColor = "lightgray"
        self.tabSelectbgColor = "lightgray"
        self.bottomTabsbgColor = "lightgray"
        self.rightTabsbgColor = "lightgray"

        self.defaultBackgroundColor = "lightgray"
        self.defaultButtonColor = "lightgray"

        self.tabselectcolor = "lightblue"
        self.tabactivecolor = "lightgrey"

        self.defaultFont1 = ("Arial",15,"bold")
        self.defaultFont1s = ("Arial",13,"bold")

        # Default Label Style
        self.style.configure("TLabel", background = self.defaultBackgroundColor)
        self.window.option_add("*Label.background",self.defaultBackgroundColor)

        # Default Button Style
        self.window.option_add("*Button.background",self.defaultButtonColor)

        # Tabs Style
        self.tabStyleName = "std.TNotebook.Tab"
        self.notebookStyleName = "std.TNotebook"
        self.style.configure(self.notebookStyleName, background = self.bottomTabsbgColor , borderwidth = 3, relief = "ridge")
        self.style.configure(self.tabStyleName, 
                             font = ("Arial" , 15), 
                             focuscolor = "", 
                             highlightthickness = 0)
        
        self.style.map(self.tabStyleName , background = [("selected" , self.tabselectcolor) , ("active" , self.tabactivecolor)])
        self.TabStyleSmallFont = 14
        self.TabStyleBigFont = 17

        # Right Tab Style
        self.RTabStyleName = "rtab.TNotebook.Tab"
        self.RNoteStyleName = "rtab.TNotebook"
        self.style.configure(self.RNoteStyleName, background = self.rightTabsbgColor, borderwidth = 3, relief = "ridge")
        self.style.configure(self.RTabStyleName ,
                             font = ("Arial" , 16),
                             focuscolor = "",
                             highlightthickness = 0)
        
        self.style.map(self.RTabStyleName , background = [("selected" , self.tabselectcolor) , ("active" , self.tabactivecolor)])
        self.RightTabBigFont = 16
        self.RightTabSmallFont = 10

        # Spinbox Big
        self.BigSpinboxStyleName = "big.TSpinbox"
        self.style.configure(self.BigSpinboxStyleName, font = ("Arial",55), padding = (5,5,5,5))
        
# endregion

# region: events

    def eventBinds(self):
        self.window.bind("<Configure>", self.on_Resize)

    def on_Resize(self , event):
        width = self.window.winfo_width()
        if width <= 700:
            self.style.configure(self.RTabStyleName, font = ("Arial" , self.RightTabSmallFont))
        else:
            self.style.configure(self.RTabStyleName, font = ("Arial" , self.RightTabBigFont))
        if width <= 600:
            self.style.configure(self.tabStyleName, font = ("Arial" , self.TabStyleSmallFont))
        else:
            self.style.configure(self.tabStyleName, font = ("Arial" , self.TabStyleBigFont))

# endregion

# region: ----- WINDOW SETUP ------
# Window Setup And Logic

    def setupWindow(self):
        self.mainbgColor = "lightgray"
        self.startingWindowLength = 600
        self.startingWindowWidth = 1000
        self.window = tk.Tk()
        self.window.title("RadCat")
        self.window.geometry("1000x600")
        self.window.configure(bg = self.mainbgColor)
        self.window.resizable(True, True)
        self.window.minsize(500, 800)
        self.window.maxsize(1920, 1080)
        self.window.protocol("WM_DELETE_WINDOW", self.onWindowClose)

    def onWindowClose(self):
        print("Window is closing...")
        self.window.destroy()

    def setupMainFrames(self):
        genLeftPad = 5
        #top menu panel
        topMenuFrameHeight = 30
        topMenuPad = 5
        topMenuRelHight = (topMenuFrameHeight / self.startingWindowLength)
        self.topMenuFrame = tk.Frame(self.window, bg = self.tabSelectbgColor, height = topMenuFrameHeight, bd = 3, relief = "ridge")
        self.topMenuFrame.pack(side = "top" , fill = "x", padx = topMenuPad, pady = topMenuPad, ipadx = topMenuPad, ipady = topMenuPad)
        topMenuEnd = topMenuFrameHeight + 4*topMenuPad

        #graph panel
        graphRelHeight = 0.5
        graphRelWidth = 0.7
        self.graphFrame = tk.Frame(self.window, bg = self.tabSelectbgColor, bd = 3, relief = "ridge")
        self.graphFrame.place(x = genLeftPad , y = topMenuEnd, relwidth = graphRelWidth , relheight = graphRelHeight)

        #right panel
        relativeSpaceFromGraph = 0.04
        self.rightFrame = tk.Frame(self.window, bg = self.tabSelectbgColor , bd = 3, relief = "ridge", width = 100)
        self.rightFrame.place(relx = graphRelWidth + relativeSpaceFromGraph , y = topMenuEnd, relwidth = 1 - graphRelWidth - 2*relativeSpaceFromGraph, relheight = 0.92)

        #bottom panel
        bottomPanelPadding = 0.02
        bottomPanelRelHight = 1 - graphRelHeight - 2 * bottomPanelPadding - topMenuRelHight
        bottomPanelRely = graphRelHeight + bottomPanelPadding + topMenuRelHight
        self.bottomFrame = tk.Frame(self.window, bg = self.tabSelectbgColor, bd = 3, relief = "ridge")
        self.bottomFrame.place(rely = bottomPanelRely , x = genLeftPad , relheight = bottomPanelRelHight , relwidth = graphRelWidth)

# endregion


# region: block functions
    def savePositionSpinboxValue(self):
        self.positionSpinboxValue = self.positionspinbox.get()
        self.testlabel.config(text = "Test:" + str(self.positionSpinboxValue))
        
    def saveAngleSpinboxValue(self):
        self.angleSpinboxValue = self.anglespinbox.get()

    def OnChannelChange(self):
        self.currentChannel = self.channelSelector.get()

    def OnHighVoltageChange(self, *args):
        self.highVoltageVar = self.HighVoltageSelector.get()

    def OnCurrentChange(self, *args):
        self.currentVar = self.CurrentSelector.get()

    def OnPressConnectMinix(self, *args):
        return

    def OnPressVoltageOn(self, *args):
        return
    
    def OnPressVoltageOff(self, *args):
        return
    
    def UItestButton(self):
        if self.controllerClass is not None:
            self.controllerClass.testButton()




# endregion

# region: ---------- BLOCK SETUP -----------
    
    # region: ---- BOTTOM BLOCK ----
    def setupBottomFrame(self):
        self.bottomFrameNotebook = ttk.Notebook(self.bottomFrame, style = self.notebookStyleName)
        self.bottomFrameNotebook.pack(fill = "both", expand = True)


        #region: ---- POSITION TAB ----
        positionTab = tk.Frame(self.bottomFrameNotebook, bg = self.bottomTabsbgColor)
        self.bottomFrameNotebook.add(positionTab, text = "Position")

        # --- DISTANCE BOX ---
        self.positionSpinboxValue = 0
        tdistance = tk.Label(positionTab, text = "Target Detector Distance", font = self.defaultFont1)
        tdistance.place(x=10, y=10)
        self.positionspinbox = ttk.Spinbox(positionTab, from_=0, to=50,increment=1,width=30, style = self.BigSpinboxStyleName)
        self.positionspinbox.place(x=10, y=40)
        tdistancesublabel = tk.Label(positionTab, text = "mm", font = self.defaultFont1)
        tdistancesublabel.place(x=220, y=40)
        getdistancebutton = tk.Button(positionTab, text = "GO", font = self.defaultFont1, bg = self.bottomTabsbgColor, command=self.savePositionSpinboxValue)
        getdistancebutton.place(x=270, y=35)

        self.testlabel = tk.Label(positionTab, text = "Test:", font = self.defaultFont1)
        self.testlabel.place(x=330, y=40)

        # --- ANGLE BOX ---
        self.angleSpinboxValue = 0
        tangle = tk.Label(positionTab, text = "Source Target Angle", font = self.defaultFont1)
        tangle.place(x=10, y=100)
        self.anglespinbox = ttk.Spinbox(positionTab, from_=0, to=360, increment=1,width=30, style = self.BigSpinboxStyleName)
        self.anglespinbox.place(x=10, y=130)
        tanglesublabel = tk.Label(positionTab, text = "mm", font  = self.defaultFont1)
        tanglesublabel.place(x=220, y=130)
        getanglebutton = tk.Button(positionTab, text = "GO", font = self.defaultFont1, bg = self.bottomTabsbgColor,command=self.saveAngleSpinboxValue)
        getanglebutton.place(x=270, y=125)
    
        # endregion

        #region: ---- DETECTOR TAB ----
        detectorTab = tk.Frame(self.bottomFrameNotebook, bg = self.bottomTabsbgColor)
        self.bottomFrameNotebook.add(detectorTab, text = "Detector")


        #endregion

        #region: ---- XRAY TAB ----
        xRayTab = tk.Frame(self.bottomFrameNotebook, bg =  self.bottomTabsbgColor)
        self.bottomFrameNotebook.add(xRayTab, text = "xRay")

        #Voltage/Current Selectors
        highVoltageText = tk.Label(xRayTab, text = "High Voltage", font = self.defaultFont1s)
        highVoltageText.place(x=10, y=10)
        highVoltageSubText = tk.Label(xRayTab, text = "kV", font = self.defaultFont1s)
        highVoltageSubText.place(x=213, y=11)

        CurrentText = tk.Label(xRayTab, text = "Current", font = self.defaultFont1s)
        CurrentText.place(x=10, y=50)
        CurrentSubText = tk.Label(xRayTab, text = "uA", font = self.defaultFont1s)
        CurrentSubText.place(x=213, y=45)

        hvstartingvalue = 15
        self.dumphvvar = tk.StringVar(value = hvstartingvalue)
        self.dumphvvar.trace_add("write",self.OnHighVoltageChange)
        self.HighVoltageSelector = ttk.Spinbox(xRayTab, from_=0, to=100, increment=1,width=10, textvariable = self.dumphvvar)
        self.HighVoltageSelector.place(x=125, y=15)
        self.HighVoltageSelector.set(hvstartingvalue)

        currentstartingvalue = 15
        self.dumpcurrentvar = tk.StringVar(value = currentstartingvalue)
        self.dumpcurrentvar.trace_add("write",self.OnCurrentChange)
        self.CurrentSelector = ttk.Spinbox(xRayTab, from_=0, to=100, increment=1, width=10, textvariable = self.dumpcurrentvar)
        self.CurrentSelector.place(x=125, y=45)
        self.CurrentSelector.set(currentstartingvalue)

        #Power and Temperature
        powertext = tk.Label(xRayTab, text = "Power", font = self.defaultFont1s)
        powertext.place(x=10, y=90)
        powersubtext = tk.Label(xRayTab, text = "mW", font = self.defaultFont1s)
        powersubtext.place(x=160 , y=90)
        temptext = tk.Label(xRayTab, text = "Temperature", font = self.defaultFont1s)
        temptext.place(x=10, y=120)
        tempsubtext = tk.Label(xRayTab, text = "Deg C", font = self.defaultFont1s)
        tempsubtext.place(x=160, y=120)

        self.powerInfoBox = tk.Label(xRayTab, text = "Off", font = self.defaultFont1s, bd = 3, relief = "ridge")
        self.powerInfoBox.place(x=125 , y=90)
        self.temperatureInfoBox = tk.Label(xRayTab, text = "Off", font = self.defaultFont1s, bd = 3, relief = "ridge")
        self.temperatureInfoBox.place(x=125 , y=120)

        #Connect ONOF
        self.ConnectButton = tk.Button(xRayTab, text = "Connect Mini-X", font = ("Arial",20,"bold"), command = self.OnPressConnectMinix)
        self.ConnectButton.place(x=40, y=160)
        self.HVONButton = tk.Button(xRayTab, text = "Voltage ON", font = ("Arial",15,"bold"), bg = "green", command = self.OnPressVoltageOn)
        self.HVONButton.place(x=20 , y= 220)
        self.HVOFFButton = tk.Button(xRayTab, text = "Voltage OFF", font = ("Arial",15,"bold"), bg = "red", command = self.OnPressVoltageOff)
        self.HVOFFButton.place(x=150, y=220)
        self.testButton = tk.Button(xRayTab, text = "Test", font = ("Arial",15,"bold"), bg = "yellow", command = self.UItestButton)
        self.testButton.place(x=100, y=270)

        #endregion

        #region: ---- MESUREMENT TAB ----

        mesurementTab = tk.Frame(self.bottomFrameNotebook, bg = self.bottomTabsbgColor)
        self.bottomFrameNotebook.add(mesurementTab, text = "Mesurements")
        channellabel = tk.Label(mesurementTab, text = "Channel", font = self.defaultFont1, bg = self.bottomTabsbgColor)
        channellabel.place(x=10, y=10)
        
        #Channel Selector
        channeloptions = ["128","1280"] #CHANNEL OPTIONS CHANGE HERE
        self.channelSelector = ttk.Combobox(mesurementTab, value = channeloptions, state = "readonly", font = self.defaultFont1)
        self.channelSelector.place(x=100, y=14)
        self.channelSelector.set("Select Channel")
        self.channelSelector.bind("<<ComboboxSelected>>", self.OnChannelChange)

        #Preset Time
        self.presetTime = tk.Label(mesurementTab, text = "Preset Time", font = self.defaultFont1, bg = self.bottomTabsbgColor)
        self.presetTime.place(x=10, y=50)

        #DeadTime
        self.deadTime = tk.Label(mesurementTab)







        #endregion

    #endregion

    def setupRightFrame(self):
        self.rightFrameNotebook = ttk.Notebook(self.rightFrame, style = self.RNoteStyleName)
        self.rightFrameNotebook.pack(fill = "both", expand = True)

        peeksTab = tk.Frame(self.rightFrameNotebook, bg = self.rightTabsbgColor)
        self.rightFrameNotebook.add(peeksTab, text = "Peeks")

        elementsTab = tk.Frame(self.rightFrameNotebook, bg = self.rightTabsbgColor)
        self.rightFrameNotebook.add(elementsTab, text = "Elements")
    
    def setupTopGraph(self):
        return
    
    def setupTopMenu(self):
        return

# endregion

# region: ---- Wrapper Functions ----

    def OnPressConnectMinix(self):
        if self.controllerClass is not None:
            self.controllerClass.connectMiniX()
            self.tryingToConnectMinix = True
            self.minixDeviceFound = False
            self.connectedToMinix = False
            self.ConnectButton.config(text = "Trying to find Mini-X...", state = "disabled", bg="yellow")
        self.window.after(500, self.CheckConnectionToMinixSetup)

    def CheckConnectionToMinixSetup(self):
        if self.controllerClass is None:
            print("BIG ERROR: Controller class not linked to UI")
            return
        if self.controllerClass.connectedToMinix == True:
            self.connectedToMinix = True
            self.tryingToConnectMinix = False
            self.ConnectButton.config(text="Connected", state="disabled", bg="green")
            return
        if self.controllerClass.connectedToMinix == False and self.controllerClass.tryingToConnectMinix == False:
            if self.debug:
                print("Not trying to connect to Mini-X")
            self.tryingToConnectMinix = False
            self.connectedToMinix = False
            self.ConnectButton.config(text="Not Connected, Try again", state="normal", bg="red")
            return
        if self.controllerClass.minixDeviceFound == True:
            self.ConnectButton.config(text="Device Found. Connecting...", state="disabled", bg="yellow")
            self.window.after(500, self.CheckConnectionToMinixSetup)
            return
        elif self.controllerClass.minixOpened == True:
            self.ConnectButton.config(text="Device Opened. Initializing...", state="disabled", bg="yellow")
            self.window.after(500, self.CheckConnectionToMinixSetup)
            return

        

    def TryFetchingDataMiniX(self):
        if self.controllerClass is None:
            print("BIG ERROR: Controller class not linked to UI")
            return
        if self.connectedToMinix == True:
            if self.controllerClass.newDataAvailable:
                # Calculate power from voltage and current
                voltage = self.controllerClass.latestVoltage  # kV
                current = self.controllerClass.latestCurrent  # µA
                temperature = self.controllerClass.latestTemperature  # °C
                
                # Calculate power: P = V * I (convert units: kV * µA = mW)
                power = voltage * current  # mW
                
                # Update UI with proper formatting
                self.powerInfoBox.config(text=f"{power:.1f}")
                self.temperatureInfoBox.config(text=f"{temperature:.1f}")
                
                # Update connection status
                if self.controllerClass.isDeviceOpen and self.tryingToConnectMinix:
                    self.connectedToMinix = True
                    self.tryingToConnectMinix = False
                    self.ConnectButton.config(text="Connected", state="normal", bg="green")
            else:
                # No new data, just check connection status
                if self.controllerClass.isDeviceOpen:
                    self.ConnectButton.config(text="Connected", state="normal", bg="green")
        else:
            if self.tryingToConnectMinix == True:
                if self.debug:
                    print("Trying to connect to Mini-X...")
            elif self.tryingToConnectMinix == False:
                if self.debug:
                    print("Not connected to Mini-X.")

        self.window.after(500, self.TryFetchingDataMiniX)


# endregion