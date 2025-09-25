VERSION 5.00
Begin VB.Form frmMiniX 
   Caption         =   "Mini-X Controller"
   ClientHeight    =   5790
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   6765
   Icon            =   "frmMiniX.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   ScaleHeight     =   386
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   451
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox picIsoCurve 
      AutoSize        =   -1  'True
      BorderStyle     =   0  'None
      Height          =   1395
      Index           =   1
      Left            =   3960
      Picture         =   "frmMiniX.frx":0442
      ScaleHeight     =   93
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   172
      TabIndex        =   38
      Top             =   3840
      Visible         =   0   'False
      Width           =   2580
   End
   Begin VB.PictureBox picXRayOn 
      AutoSize        =   -1  'True
      BorderStyle     =   0  'None
      Height          =   480
      Left            =   3120
      Picture         =   "frmMiniX.frx":BFF8
      ScaleHeight     =   480
      ScaleWidth      =   480
      TabIndex        =   36
      Top             =   840
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Timer tmrXRayOn 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   1080
      Top             =   840
   End
   Begin VB.Timer tmrMonitor 
      Enabled         =   0   'False
      Interval        =   1500
      Left            =   600
      Top             =   840
   End
   Begin VB.Timer tmrInterLock 
      Enabled         =   0   'False
      Interval        =   4000
      Left            =   120
      Top             =   840
   End
   Begin VB.PictureBox picInterLock 
      AutoSize        =   -1  'True
      BorderStyle     =   0  'None
      Height          =   480
      Index           =   1
      Left            =   5880
      Picture         =   "frmMiniX.frx":C302
      ScaleHeight     =   480
      ScaleWidth      =   480
      TabIndex        =   33
      ToolTipText     =   "INTERLOCK RESTORED"
      Top             =   840
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.PictureBox picInterLock 
      AutoSize        =   -1  'True
      BorderStyle     =   0  'None
      Height          =   480
      Index           =   0
      Left            =   5880
      Picture         =   "frmMiniX.frx":C60C
      ScaleHeight     =   480
      ScaleWidth      =   480
      TabIndex        =   32
      ToolTipText     =   "INTERLOCK OPEN"
      Top             =   840
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Frame fraSettings 
      Caption         =   "Set/Monitor High Voltage and Current"
      Height          =   2175
      Left            =   120
      TabIndex        =   11
      Top             =   1650
      Width           =   6495
      Begin VB.CommandButton cmdSetHVandCurrent 
         Caption         =   "Set High Voltage and Current"
         Enabled         =   0   'False
         Height          =   615
         Left            =   360
         TabIndex        =   31
         Top             =   600
         Width           =   1455
      End
      Begin VB.TextBox txtCurrent 
         Height          =   285
         Left            =   1920
         TabIndex        =   18
         Text            =   "15"
         Top             =   960
         Width           =   855
      End
      Begin VB.TextBox txtHv 
         Height          =   285
         Left            =   1920
         TabIndex        =   15
         Text            =   "15"
         Top             =   600
         Width           =   855
      End
      Begin VB.CommandButton cmdExit 
         Cancel          =   -1  'True
         Height          =   675
         Left            =   3840
         Picture         =   "frmMiniX.frx":C916
         Style           =   1  'Graphical
         TabIndex        =   14
         Top             =   1380
         Width           =   2535
      End
      Begin VB.CommandButton cmdHvOff 
         Enabled         =   0   'False
         Height          =   540
         Left            =   4800
         Picture         =   "frmMiniX.frx":CAC3
         Style           =   1  'Graphical
         TabIndex        =   13
         Top             =   180
         Width           =   1575
      End
      Begin VB.CommandButton cmdHvOn 
         Enabled         =   0   'False
         Height          =   540
         Left            =   4800
         Picture         =   "frmMiniX.frx":CC1C
         Style           =   1  'Graphical
         TabIndex        =   12
         Top             =   780
         Width           =   1575
      End
      Begin VB.Label lblWattageOutOfRange 
         Alignment       =   2  'Center
         BackStyle       =   0  'Transparent
         Caption         =   ">> Wattage Out Of Range <<"
         ForeColor       =   &H00808000&
         Height          =   255
         Left            =   360
         TabIndex        =   35
         Top             =   240
         Visible         =   0   'False
         Width           =   2415
      End
      Begin VB.Label lbllblMonitor 
         Alignment       =   1  'Right Justify
         BackStyle       =   0  'Transparent
         Caption         =   "Current Monitor"
         Height          =   255
         Index           =   1
         Left            =   120
         TabIndex        =   30
         Top             =   1800
         Width           =   1695
      End
      Begin VB.Label lbllblMonitor 
         Alignment       =   1  'Right Justify
         BackStyle       =   0  'Transparent
         Caption         =   "High Voltage Monitor"
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   29
         Top             =   1440
         Width           =   1695
      End
      Begin VB.Label lbluA 
         Alignment       =   2  'Center
         BackStyle       =   0  'Transparent
         Caption         =   "uA"
         Height          =   255
         Index           =   1
         Left            =   2880
         TabIndex        =   28
         Top             =   1800
         Width           =   375
      End
      Begin VB.Label lbluA 
         Alignment       =   2  'Center
         BackStyle       =   0  'Transparent
         Caption         =   "uA"
         Height          =   255
         Index           =   0
         Left            =   4080
         TabIndex        =   27
         Top             =   960
         Width           =   375
      End
      Begin VB.Label lblkV 
         Alignment       =   2  'Center
         BackStyle       =   0  'Transparent
         Caption         =   "kV"
         Height          =   255
         Index           =   1
         Left            =   2880
         TabIndex        =   26
         Top             =   1440
         Width           =   375
      End
      Begin VB.Label lblkV 
         Alignment       =   2  'Center
         BackStyle       =   0  'Transparent
         Caption         =   "kV"
         Height          =   255
         Index           =   0
         Left            =   4080
         TabIndex        =   25
         Top             =   600
         Width           =   375
      End
      Begin VB.Label lblCurrentMonitor 
         BorderStyle     =   1  'Fixed Single
         Caption         =   "0.0"
         Height          =   255
         Left            =   1920
         TabIndex        =   24
         Top             =   1800
         Width           =   855
      End
      Begin VB.Label lblHvMonitor 
         BorderStyle     =   1  'Fixed Single
         Caption         =   "0.0"
         Height          =   255
         Left            =   1920
         TabIndex        =   23
         Top             =   1440
         Width           =   855
      End
      Begin VB.Label lblMAX 
         Alignment       =   2  'Center
         BackStyle       =   0  'Transparent
         Caption         =   "200"
         Height          =   255
         Index           =   2
         Left            =   3480
         TabIndex        =   22
         Top             =   960
         Width           =   495
      End
      Begin VB.Label lblMAX 
         Alignment       =   2  'Center
         BackStyle       =   0  'Transparent
         Caption         =   "40"
         Height          =   255
         Index           =   1
         Left            =   3480
         TabIndex        =   21
         Top             =   600
         Width           =   495
      End
      Begin VB.Label lblMIN 
         Alignment       =   2  'Center
         BackStyle       =   0  'Transparent
         Caption         =   "5"
         Height          =   255
         Index           =   2
         Left            =   2880
         TabIndex        =   20
         Top             =   960
         Width           =   495
      End
      Begin VB.Label lblMIN 
         Alignment       =   2  'Center
         BackStyle       =   0  'Transparent
         Caption         =   "10"
         Height          =   255
         Index           =   1
         Left            =   2880
         TabIndex        =   19
         Top             =   600
         Width           =   495
      End
      Begin VB.Label lblMAX 
         Alignment       =   2  'Center
         BackStyle       =   0  'Transparent
         Caption         =   "MAX"
         Height          =   255
         Index           =   0
         Left            =   3480
         TabIndex        =   17
         Top             =   240
         Width           =   495
      End
      Begin VB.Label lblMIN 
         Alignment       =   2  'Center
         BackStyle       =   0  'Transparent
         Caption         =   "MIN"
         Height          =   255
         Index           =   0
         Left            =   2880
         TabIndex        =   16
         Top             =   240
         Width           =   495
      End
   End
   Begin VB.CommandButton cmdSTART_MINIX_CONTROLLER 
      Caption         =   "Start Amptek Mini-X"
      Height          =   375
      Left            =   1680
      TabIndex        =   4
      Top             =   120
      Width           =   3255
   End
   Begin VB.PictureBox picIsoCurve 
      AutoSize        =   -1  'True
      BorderStyle     =   0  'None
      Height          =   1395
      Index           =   0
      Left            =   3960
      Picture         =   "frmMiniX.frx":CD6E
      ScaleHeight     =   93
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   172
      TabIndex        =   3
      Top             =   3840
      Width           =   2580
   End
   Begin VB.PictureBox picMiniXHw 
      AutoSize        =   -1  'True
      BorderStyle     =   0  'None
      Height          =   1395
      Left            =   120
      Picture         =   "frmMiniX.frx":18924
      ScaleHeight     =   93
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   190
      TabIndex        =   2
      Top             =   3840
      Width           =   2850
   End
   Begin VB.PictureBox picMiniXLogo 
      AutoSize        =   -1  'True
      BorderStyle     =   0  'None
      Height          =   675
      Left            =   5160
      Picture         =   "frmMiniX.frx":1D326
      ScaleHeight     =   45
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   98
      TabIndex        =   1
      Top             =   105
      Width           =   1470
   End
   Begin VB.PictureBox picAmptekLogo 
      AutoSize        =   -1  'True
      BorderStyle     =   0  'None
      Height          =   675
      Left            =   105
      Picture         =   "frmMiniX.frx":1E8FC
      ScaleHeight     =   45
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   92
      TabIndex        =   0
      Top             =   105
      Width           =   1380
   End
   Begin VB.Label lblPower_mW 
      Alignment       =   2  'Center
      BackColor       =   &H00FFFFFF&
      BorderStyle     =   1  'Fixed Single
      Height          =   255
      Left            =   3120
      TabIndex        =   8
      Top             =   4920
      Width           =   735
   End
   Begin VB.Label lblXRayOn 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      Caption         =   "X-RAY ON"
      Height          =   255
      Left            =   2880
      TabIndex        =   37
      Top             =   1380
      Visible         =   0   'False
      Width           =   975
   End
   Begin VB.Label lblInterLock 
      Alignment       =   1  'Right Justify
      BackStyle       =   0  'Transparent
      Caption         =   "INTERLOCK RESTORED"
      Height          =   255
      Left            =   3720
      TabIndex        =   34
      Top             =   1080
      Width           =   2055
   End
   Begin VB.Label lblSerialNumber 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      Caption         =   "lblSerialNumber"
      Height          =   255
      Left            =   1680
      TabIndex        =   10
      Top             =   525
      Width           =   3255
   End
   Begin VB.Label lblBoardTemp 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      BorderStyle     =   1  'Fixed Single
      Height          =   255
      Left            =   3120
      TabIndex        =   9
      Top             =   4320
      Width           =   735
   End
   Begin VB.Label lbllblPower_mW 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      Caption         =   "Power mW"
      Height          =   255
      Left            =   3060
      TabIndex        =   7
      Top             =   4680
      Width           =   855
   End
   Begin VB.Label lbllblBoardTemp 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      Caption         =   "Board Temp.°C"
      Height          =   495
      Left            =   3120
      TabIndex        =   6
      Top             =   3840
      Width           =   735
   End
   Begin VB.Label lblStatus 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      Height          =   375
      Left            =   120
      TabIndex        =   5
      Top             =   5280
      Width           =   6495
   End
End
Attribute VB_Name = "frmMiniX"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)

Private indInterlockStatus As Boolean
Private indDisableMonitorCmds As Boolean

Private Sub cmdExit_Click()
    On Error Resume Next
    If (CBool(isMiniXDlg())) Then
        tmrMonitor.Enabled = False
        SendMiniXCommand CByte(mxcExit)
        Sleep 100
    End If
    Unload Me
End Sub

Private Sub cmdHvOff_Click()
    indDisableMonitorCmds = True
    cmdHvOff.Enabled = False
    If (CBool(isMiniXDlg())) Then
        SendMiniXCommand CByte(mxcHVOff)
        Sleep 100
    End If
    indDisableMonitorCmds = False
End Sub

Private Sub cmdHvOn_Click()
    Dim msgResponse As Integer
    indDisableMonitorCmds = True
    cmdHvOn.Enabled = False
    If (CBool(isMiniXDlg())) Then
        msgResponse = MsgBox("Turn X-RAY High Voltage ON?", vbYesNo + vbQuestion + vbSystemModal, "MiniX")
        If msgResponse = vbYes Then
            SendMiniXCommand CByte(mxcHVOn)
            Sleep 100
        End If
    End If
    indDisableMonitorCmds = False
End Sub

Private Sub cmdSetHVandCurrent_Click()
    Dim dblValue As Double
    Dim MiniXSettings As MiniX_Settings
    indDisableMonitorCmds = True
    cmdSetHVandCurrent.Enabled = False
    cmdHvOn.Enabled = False
    cmdHvOff.Enabled = False
    If (CBool(isMiniXDlg())) Then
        dblValue = CDbl(txtHv.Text)         'sent hv value
        SetMiniXHV dblValue
        Sleep 100
        dblValue = CDbl(txtCurrent.Text)    'send current value
        SetMiniXCurrent dblValue
        Sleep 100
        SendMiniXCommand CByte(mxcSetHVandCurrent)
        Sleep 1000
        ReadMiniXSettings MiniXSettings     'read the corrected values back
        txtHv.Text = CStr(MiniXSettings.HighVoltage_kV)
        txtCurrent.Text = CStr(MiniXSettings.Current_uA)
    End If
    indDisableMonitorCmds = False
End Sub

Private Sub cmdSTART_MINIX_CONTROLLER_Click()
    Dim MiniXSerialNumber As Long
    Dim strSerialNumber As String
    
    '==== MiniXSerialNumber >= 10000 and < 1118880 40kV Tube ====
    '==== MiniXSerialNumber >= 1118880 50kV Tube ====
    picIsoCurve(0).Visible = True
    picIsoCurve(1).Visible = False
    lblMAX(1).Caption = "40"

    indDisableMonitorCmds = True
    cmdSTART_MINIX_CONTROLLER.Enabled = False
    If (CBool(isMiniXDlg())) Then
        SendMiniXCommand CByte(mxcStartMiniX)
        Sleep 100
        tmrMonitor.Enabled = True
    End If
    If (CBool(isMiniXDlg())) Then
        Sleep 100
        MiniXSerialNumber = ReadMiniXSerialNumber()
        If (MiniXSerialNumber > 0) Then
            strSerialNumber = Format(MiniXSerialNumber, "00000000")
            strSerialNumber = "Mini-X Serial Number " & strSerialNumber
            lblSerialNumber.Caption = strSerialNumber
            If (MiniXSerialNumber >= 1118880) Then  '50kV Tube
                picIsoCurve(0).Visible = False
                picIsoCurve(1).Visible = True
                lblMAX(1).Caption = "50"
            End If

        Else
            lblSerialNumber.Caption = ""
        End If
    End If
    indDisableMonitorCmds = False
End Sub

Private Sub Form_Initialize()
    InitXpStyles
End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
    Dim AltDown As Boolean
    AltDown = CBool((Shift And vbAltMask) > 0)
    If (AltDown And (KeyCode = vbKeyX)) Then
        cmdExit_Click
    End If
End Sub

Private Sub Form_Load()
    Dim MiniXSettings As MiniX_Settings
    lblSerialNumber.Caption = ""
    lblInterLock.Caption = ""
    
    If (Not CBool(isMiniXDlg())) Then
        OpenMiniX
        Sleep 100
    End If
    If (CBool(isMiniXDlg())) Then
        ReadMiniXSettings MiniXSettings
        txtHv.Text = CStr(MiniXSettings.HighVoltage_kV)
        txtCurrent.Text = CStr(MiniXSettings.Current_uA)
    End If
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    If (CBool(isMiniXDlg())) Then
        CloseMiniX
        Sleep 100
    End If
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Unload Me
End Sub

Private Sub tmrInterLock_Timer()
    picInterLock(0).Visible = False
    picInterLock(1).Visible = False
    lblInterLock.Caption = ""
End Sub

Private Sub UpdateMonitor()
    Dim MiniXMonitor As MiniX_Monitor
    Dim strMonitor As String
    Dim indOldInterlockStatus As Boolean

    If (CBool(isMiniXDlg())) Then
        ReadMiniXMonitor MiniXMonitor
        Sleep 100
        If (MiniXMonitor.mxmRefreshed) Then             'test if the status was updated
        If (MiniXMonitor.mxmReserved = 123.456) Then    'reserved but always 123.456
            lblHvMonitor.Caption = Format(MiniXMonitor.mxmHighVoltage_kV, "0.0")
            lblCurrentMonitor.Caption = Format(MiniXMonitor.mxmCurrent_uA, "0.0")
            lblPower_mW.Caption = Format(MiniXMonitor.mxmPower_mW, "0")
            lblBoardTemp.Caption = Format(MiniXMonitor.mxmTemperatureC, "0")
            lblStatus.Caption = GetMiniXStatusString(MiniXMonitor.mxmStatusInd)
            indOldInterlockStatus = indInterlockStatus
            indInterlockStatus = CBool(MiniXMonitor.mxmInterLock)
            If (indOldInterlockStatus <> indInterlockStatus) Then
                If (indInterlockStatus) Then
                    picInterLock(0).Visible = False
                    picInterLock(1).Visible = True
                    lblInterLock.Caption = picInterLock(1).ToolTipText
                    tmrInterLock.Enabled = True
                Else
                    picInterLock(0).Visible = True
                    picInterLock(1).Visible = False
                    lblInterLock.Caption = picInterLock(0).ToolTipText
                    tmrInterLock.Enabled = False
                End If
            End If
            EnableMiniX_Commands MiniXMonitor.mxmEnabledCmds
            If (MiniXMonitor.mxmHVOn) Then
                If (Not tmrXRayOn.Enabled) Then tmrXRayOn.Enabled = True
            Else
                If (tmrXRayOn.Enabled) Then tmrXRayOn.Enabled = False
                If (picXRayOn.Visible) Then picXRayOn.Visible = False
                If (lblXRayOn.Visible) Then lblXRayOn.Visible = False
            End If
            lblWattageOutOfRange.Visible = MiniXMonitor.mxmOutOfRange
        End If
        End If
    End If

End Sub

Private Sub EnableMiniX_Commands(mxmEnabledCmds As Byte)
    Dim indStartMiniX As Boolean
    Dim indHVOn As Boolean
    Dim indHVOff As Boolean
    Dim indSetHVandCurrent As Boolean

    If (Not indDisableMonitorCmds) Then
        indStartMiniX = CBool(mxmEnabledCmds And mxcStartMiniX)
        indHVOn = CBool(mxmEnabledCmds And mxcHVOn)
        indHVOff = CBool(mxmEnabledCmds And mxcHVOff)
        indSetHVandCurrent = CBool(mxmEnabledCmds And mxcSetHVandCurrent)
        cmdSTART_MINIX_CONTROLLER.Enabled = indStartMiniX
        cmdHvOn.Enabled = indHVOn
        cmdHvOff.Enabled = indHVOff
        cmdSetHVandCurrent.Enabled = indSetHVandCurrent
    End If
End Sub

Private Sub tmrMonitor_Timer()
    UpdateMonitor
End Sub

Private Sub tmrXRayOn_Timer()
    lblXRayOn.Visible = True
    If (picXRayOn.Visible) Then
        picXRayOn.Visible = False
    Else
        picXRayOn.Visible = True
    End If
End Sub



