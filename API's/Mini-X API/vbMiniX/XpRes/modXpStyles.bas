Attribute VB_Name = "modXpStyles"
Option Explicit

'Example Usage:

'Private Sub Form_Initialize()
'    InitXpStyles
'End Sub

'This can either be in the same folder as your executable called appname.exe.manifest
'or it can be embedded in a resource file with a type of 24 and an ID of 1.

'Now, to make these work properly, you need to call the InitCommonControls function
'before any forms are loaded. This can either be in the startup forms Initialize event or in Sub Main()

Public Type tagInitCommonControlsEx
   lngSize As Long
   lngICC As Long
End Type
Public Declare Function InitCommonControlsEx Lib "comctl32.dll" (iccex As tagInitCommonControlsEx) As Boolean
Public Declare Sub InitCommonControls Lib "comctl32.dll" ()
Public Declare Function LoadLibrary Lib "kernel32" Alias "LoadLibraryA" (ByVal lpLibFileName As String) As Long
Public Const ICC_USEREX_CLASSES = &H200

Public Sub InitXpStyles()
    On Error Resume Next
    LoadLibrary "shell32.dll"
    InitCommonControls
    On Error GoTo 0
End Sub

Public Sub InitXpStylesEx()
   ' we need to call InitCommonControls before we
   ' can use XP visual styles.  Here I'm using
   ' InitCommonControlsEx, which is the extended
   ' version provided in v4.72 upwards (you need
   ' v6.00 or higher to get XP styles)
   On Error Resume Next
   LoadLibrary "shell32.dll"

   ' this will fail if Comctl not available
   Dim iccex As tagInitCommonControlsEx
   With iccex
       .lngSize = LenB(iccex)
       .lngICC = ICC_USEREX_CLASSES
   End With
   InitCommonControlsEx iccex
   
   ' now start the application
   On Error GoTo 0
   'frmTestControls.Show
End Sub

