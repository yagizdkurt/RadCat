Option Strict Off
Option Explicit On
Module modMiniX

    'holds monitored values from ReadMiniXMonitor
    Public Structure MiniX_Monitor
        Dim mxmHighVoltage_kV As Double 'high voltage monitor
        Dim mxmCurrent_uA As Double     'current monitor
        Dim mxmPower_mW As Double       'power in milliwatts
        Dim mxmTemperatureC As Double   'temperature in degrees C
        Dim mxmRefreshed As Byte        'monitor data refresh ok
        Dim mxmInterLock As Byte        '0=Open,1=Restored(Closed)
        Dim mxmEnabledCmds As Byte      'command button enables
        Dim mxmStatusInd As Byte        'minix status indicator
        Dim mxmOutOfRange As Byte       'wattage value out of range
        Dim mxmHVOn As Byte             'high voltage on indicator
        Dim mxmReserved As Double       'reserved, should be 123.456
    End Structure

    'holds corrected settings from ReadMiniXSettings
    Public Structure MiniX_Settings
        Dim HighVoltage_kV As Double    'high voltage setting
        Dim Current_uA As Double        'current setting
    End Structure

    'commands that can be sent with SendMiniXCommand
    'ALSO
    'command enable masks for mxmEnabledCmds
    Public Enum MiniX_Commands
        mxcDisabled = 0             'null command
        mxcStartMiniX = 1           'start minix controller
        mxcHVOn = 2                 'turn high voltage on
        mxcHVOff = 4                'turn high voltage off
        mxcSetHVandCurrent = 8      'set high voltage and current
        mxcExit = 16                'exit controller
    End Enum

    'minix controller status codes
    Public Enum MiniX_Status
        mxstNoStatus                    'no status available
        mxstDriversNotLoaded            'drivers were not found, install drivers
        mxstMiniXApplicationReady       'application is ready to connect to minix
        mxstPortCLOSED                  'minix detected, port closed, will attempt connect
        mxstNoDevicesAttached           'minix is not connected or is not powered
        mxstMiniXControllerSelected     'minix has been found
        mxstMiniXControllerReady        'minix connected and ready for first command
        mxstMiniXControllerFailedToOpen 'minix detected, but failed to open
        mxstNoDeviceSelected            'could not select minix device
        mxstRequestedVoltageOutOfRange  'hv was selected out of range,api will set in range
        mxstRequestedCurrentOutOfRange  'uA was selected out of range,api will set in range
        mxstConnectingToMiniX           'api busy attempting to connect to minix
        mxstUpdatingSettings            'api busy updating settings
        mxstMiniXReady                  'ready for next operation
    End Enum

    'OpenMiniX - Opens an Instance of a MiniX Controller Application
    Public Declare Sub OpenMiniX Lib "MiniX" ()

    'isMiniXDlg - Test for an Instance of a MiniX Controller Application
    Public Declare Function isMiniXDlg Lib "MiniX" () As Byte

    'CloseMiniX - Closes an Instance of a MiniX Controller Application
    Public Declare Sub CloseMiniX Lib "MiniX" ()

    'SendMiniXCommand - Sends Commands to MiniX
    Public Declare Sub SendMiniXCommand Lib "MiniX" (ByVal MiniXCommand As Byte)

    'ReadMiniXMonitor - Reads Monitored Values
    Public Declare Sub ReadMiniXMonitor Lib "MiniX" (ByRef MiniXMonitor As MiniX_Monitor)

    'SetMiniXHV - Sets Requested HighVoltage (kV)
    Public Declare Sub SetMiniXHV Lib "MiniX" (ByVal HighVoltage_kV As Double)

    'SetMiniXCurrent - Sets Requested Current (uA)
    Public Declare Sub SetMiniXCurrent Lib "MiniX" (ByVal Current_uA As Double)

    'ReadMiniXSettings - Reads Actual Values Set
    Public Declare Sub ReadMiniXSettings Lib "MiniX" (ByRef MiniXSettings As MiniX_Settings)

    'ReadMiniXSerialNumber - Read MiniX Serial Number
    Public Declare Function ReadMiniXSerialNumber Lib "MiniX" () As Integer

    'converts minix controller status codes (mxmStatusInd) to text strings
    Public Function GetMiniXStatusString(ByVal monitorMiniXStatus As Byte) As String
        Select Case monitorMiniXStatus
            Case MiniX_Status.mxstNoStatus
                GetMiniXStatusString = "No Status"
            Case MiniX_Status.mxstDriversNotLoaded
                GetMiniXStatusString = "Drivers not loaded, please install MiniX drivers."
            Case MiniX_Status.mxstMiniXApplicationReady
                GetMiniXStatusString = "MiniX application ready."
            Case MiniX_Status.mxstPortCLOSED
                GetMiniXStatusString = "Port CLOSED"
            Case MiniX_Status.mxstNoDevicesAttached
                GetMiniXStatusString = "No devices attached"
            Case MiniX_Status.mxstMiniXControllerSelected
                GetMiniXStatusString = "MiniX Controller selected"
            Case MiniX_Status.mxstMiniXControllerReady
                GetMiniXStatusString = "MiniX Controller ready"
            Case MiniX_Status.mxstMiniXControllerFailedToOpen
                GetMiniXStatusString = "MiniX Controller failed to open"
            Case MiniX_Status.mxstNoDeviceSelected
                GetMiniXStatusString = "No device selected"
            Case MiniX_Status.mxstRequestedVoltageOutOfRange
                GetMiniXStatusString = "Requested voltage out of range.  Please enter voltage in range."
            Case MiniX_Status.mxstRequestedCurrentOutOfRange
                GetMiniXStatusString = "Requested current out of range.  Please enter current in range."
            Case MiniX_Status.mxstConnectingToMiniX
                GetMiniXStatusString = "Connecting to MiniX. Please wait..."
            Case MiniX_Status.mxstUpdatingSettings
                GetMiniXStatusString = "Updating settings. Please wait..."
            Case MiniX_Status.mxstMiniXReady
                GetMiniXStatusString = "Mini-X Ready"
        End Select
    End Function
End Module