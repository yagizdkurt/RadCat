Option Strict Off
Option Explicit On

Friend Class frmMiniX
    Inherits System.Windows.Forms.Form

#Region "Windows Form Designer generated code "

    Public Sub New()
        MyBase.New()
        If m_vb6FormDefInstance Is Nothing Then
            If m_InitializingDefInstance Then
                m_vb6FormDefInstance = Me
            Else
                Try
                    'For the start-up form, the first instance created is the default instance.
                    If System.Reflection.Assembly.GetExecutingAssembly.EntryPoint.DeclaringType Is Me.GetType Then
                        m_vb6FormDefInstance = Me
                    End If
                Catch
                End Try
            End If
        End If
        'This call is required by the Windows Form Designer.
        InitializeComponent()
        System.Windows.Forms.Application.EnableVisualStyles()
    End Sub
    'Form overrides dispose to clean up the component list.
    Protected Overloads Overrides Sub Dispose(ByVal Disposing As Boolean)
        If Disposing Then
            If Not components Is Nothing Then
                components.Dispose()
            End If
        End If
        MyBase.Dispose(Disposing)
    End Sub
    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer
    Public ToolTip1 As System.Windows.Forms.ToolTip
    Public WithEvents picXRayOn As System.Windows.Forms.PictureBox
    Public WithEvents tmrXRayOn As System.Windows.Forms.Timer
    Public WithEvents tmrMonitor As System.Windows.Forms.Timer
    Public WithEvents tmrInterLock As System.Windows.Forms.Timer
    Public WithEvents _picInterLock_1 As System.Windows.Forms.PictureBox
    Public WithEvents _picInterLock_0 As System.Windows.Forms.PictureBox
    Public WithEvents cmdSetHVandCurrent As System.Windows.Forms.Button
    Public WithEvents txtCurrent As System.Windows.Forms.TextBox
    Public WithEvents txtHv As System.Windows.Forms.TextBox
    Public WithEvents cmdExit As System.Windows.Forms.Button
    Public WithEvents cmdHvOff As System.Windows.Forms.Button
    Public WithEvents cmdHvOn As System.Windows.Forms.Button
    Public WithEvents lblWattageOutOfRange As System.Windows.Forms.Label
    Public WithEvents _lbllblMonitor_1 As System.Windows.Forms.Label
    Public WithEvents _lbllblMonitor_0 As System.Windows.Forms.Label
    Public WithEvents _lbluA_1 As System.Windows.Forms.Label
    Public WithEvents _lbluA_0 As System.Windows.Forms.Label
    Public WithEvents _lblkV_1 As System.Windows.Forms.Label
    Public WithEvents _lblkV_0 As System.Windows.Forms.Label
    Public WithEvents lblCurrentMonitor As System.Windows.Forms.Label
    Public WithEvents lblHvMonitor As System.Windows.Forms.Label
    Public WithEvents _lblMAX_2 As System.Windows.Forms.Label
    Public WithEvents _lblMAX_1 As System.Windows.Forms.Label
    Public WithEvents _lblMIN_2 As System.Windows.Forms.Label
    Public WithEvents _lblMIN_1 As System.Windows.Forms.Label
    Public WithEvents _lblMAX_0 As System.Windows.Forms.Label
    Public WithEvents _lblMIN_0 As System.Windows.Forms.Label
    Public WithEvents fraSettings As System.Windows.Forms.GroupBox
    Public WithEvents cmdSTART_MINIX_CONTROLLER As System.Windows.Forms.Button
    Public WithEvents picIsoCurve As System.Windows.Forms.PictureBox
    Public WithEvents picMiniXHw As System.Windows.Forms.PictureBox
    Public WithEvents picMiniXLogo As System.Windows.Forms.PictureBox
    Public WithEvents picAmptekLogo As System.Windows.Forms.PictureBox
    Public WithEvents lblPower_mW As System.Windows.Forms.Label
    Public WithEvents lblXRayOn As System.Windows.Forms.Label
    Public WithEvents lblInterLock As System.Windows.Forms.Label
    Public WithEvents lblSerialNumber As System.Windows.Forms.Label
    Public WithEvents lblBoardTemp As System.Windows.Forms.Label
    Public WithEvents lbllblPower_mW As System.Windows.Forms.Label
    Public WithEvents lbllblBoardTemp As System.Windows.Forms.Label
    Public WithEvents lblStatus As System.Windows.Forms.Label
    Public WithEvents lblMAX As Microsoft.VisualBasic.Compatibility.VB6.LabelArray
    Public WithEvents lblMIN As Microsoft.VisualBasic.Compatibility.VB6.LabelArray
    Public WithEvents lblkV As Microsoft.VisualBasic.Compatibility.VB6.LabelArray
    Public WithEvents lbllblMonitor As Microsoft.VisualBasic.Compatibility.VB6.LabelArray
    Public WithEvents lbluA As Microsoft.VisualBasic.Compatibility.VB6.LabelArray
    Public WithEvents picInterLock As Microsoft.VisualBasic.Compatibility.VB6.PictureBoxArray
    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.
    'Do not modify it using the code editor.
    Public WithEvents picIsoCurve50kV As System.Windows.Forms.PictureBox
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container
        Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(frmMiniX))
        Me.ToolTip1 = New System.Windows.Forms.ToolTip(Me.components)
        Me._picInterLock_1 = New System.Windows.Forms.PictureBox
        Me._picInterLock_0 = New System.Windows.Forms.PictureBox
        Me.picXRayOn = New System.Windows.Forms.PictureBox
        Me.tmrXRayOn = New System.Windows.Forms.Timer(Me.components)
        Me.tmrMonitor = New System.Windows.Forms.Timer(Me.components)
        Me.tmrInterLock = New System.Windows.Forms.Timer(Me.components)
        Me.fraSettings = New System.Windows.Forms.GroupBox
        Me.cmdSetHVandCurrent = New System.Windows.Forms.Button
        Me.txtCurrent = New System.Windows.Forms.TextBox
        Me.txtHv = New System.Windows.Forms.TextBox
        Me.cmdExit = New System.Windows.Forms.Button
        Me.cmdHvOff = New System.Windows.Forms.Button
        Me.cmdHvOn = New System.Windows.Forms.Button
        Me.lblWattageOutOfRange = New System.Windows.Forms.Label
        Me._lbllblMonitor_1 = New System.Windows.Forms.Label
        Me._lbllblMonitor_0 = New System.Windows.Forms.Label
        Me._lbluA_1 = New System.Windows.Forms.Label
        Me._lbluA_0 = New System.Windows.Forms.Label
        Me._lblkV_1 = New System.Windows.Forms.Label
        Me._lblkV_0 = New System.Windows.Forms.Label
        Me.lblCurrentMonitor = New System.Windows.Forms.Label
        Me.lblHvMonitor = New System.Windows.Forms.Label
        Me._lblMAX_2 = New System.Windows.Forms.Label
        Me._lblMAX_1 = New System.Windows.Forms.Label
        Me._lblMIN_2 = New System.Windows.Forms.Label
        Me._lblMIN_1 = New System.Windows.Forms.Label
        Me._lblMAX_0 = New System.Windows.Forms.Label
        Me._lblMIN_0 = New System.Windows.Forms.Label
        Me.cmdSTART_MINIX_CONTROLLER = New System.Windows.Forms.Button
        Me.picIsoCurve = New System.Windows.Forms.PictureBox
        Me.picMiniXHw = New System.Windows.Forms.PictureBox
        Me.picMiniXLogo = New System.Windows.Forms.PictureBox
        Me.picAmptekLogo = New System.Windows.Forms.PictureBox
        Me.lblPower_mW = New System.Windows.Forms.Label
        Me.lblXRayOn = New System.Windows.Forms.Label
        Me.lblInterLock = New System.Windows.Forms.Label
        Me.lblSerialNumber = New System.Windows.Forms.Label
        Me.lblBoardTemp = New System.Windows.Forms.Label
        Me.lbllblPower_mW = New System.Windows.Forms.Label
        Me.lbllblBoardTemp = New System.Windows.Forms.Label
        Me.lblStatus = New System.Windows.Forms.Label
        Me.lblMAX = New Microsoft.VisualBasic.Compatibility.VB6.LabelArray(Me.components)
        Me.lblMIN = New Microsoft.VisualBasic.Compatibility.VB6.LabelArray(Me.components)
        Me.lblkV = New Microsoft.VisualBasic.Compatibility.VB6.LabelArray(Me.components)
        Me.lbllblMonitor = New Microsoft.VisualBasic.Compatibility.VB6.LabelArray(Me.components)
        Me.lbluA = New Microsoft.VisualBasic.Compatibility.VB6.LabelArray(Me.components)
        Me.picInterLock = New Microsoft.VisualBasic.Compatibility.VB6.PictureBoxArray(Me.components)
        Me.picIsoCurve50kV = New System.Windows.Forms.PictureBox
        Me.fraSettings.SuspendLayout()
        CType(Me.lblMAX, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.lblMIN, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.lblkV, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.lbllblMonitor, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.lbluA, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.picInterLock, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        '_picInterLock_1
        '
        Me._picInterLock_1.BackColor = System.Drawing.SystemColors.Control
        Me._picInterLock_1.Cursor = System.Windows.Forms.Cursors.Default
        Me._picInterLock_1.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picInterLock_1.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picInterLock_1.Image = CType(resources.GetObject("_picInterLock_1.Image"), System.Drawing.Image)
        Me.picInterLock.SetIndex(Me._picInterLock_1, CType(1, Short))
        Me._picInterLock_1.Location = New System.Drawing.Point(392, 56)
        Me._picInterLock_1.Name = "_picInterLock_1"
        Me._picInterLock_1.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picInterLock_1.Size = New System.Drawing.Size(32, 32)
        Me._picInterLock_1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize
        Me._picInterLock_1.TabIndex = 33
        Me._picInterLock_1.TabStop = False
        Me.ToolTip1.SetToolTip(Me._picInterLock_1, "INTERLOCK RESTORED")
        Me._picInterLock_1.Visible = False
        '
        '_picInterLock_0
        '
        Me._picInterLock_0.BackColor = System.Drawing.SystemColors.Control
        Me._picInterLock_0.Cursor = System.Windows.Forms.Cursors.Default
        Me._picInterLock_0.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._picInterLock_0.ForeColor = System.Drawing.SystemColors.ControlText
        Me._picInterLock_0.Image = CType(resources.GetObject("_picInterLock_0.Image"), System.Drawing.Image)
        Me.picInterLock.SetIndex(Me._picInterLock_0, CType(0, Short))
        Me._picInterLock_0.Location = New System.Drawing.Point(392, 56)
        Me._picInterLock_0.Name = "_picInterLock_0"
        Me._picInterLock_0.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._picInterLock_0.Size = New System.Drawing.Size(32, 32)
        Me._picInterLock_0.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize
        Me._picInterLock_0.TabIndex = 32
        Me._picInterLock_0.TabStop = False
        Me.ToolTip1.SetToolTip(Me._picInterLock_0, "INTERLOCK OPEN")
        Me._picInterLock_0.Visible = False
        '
        'picXRayOn
        '
        Me.picXRayOn.BackColor = System.Drawing.SystemColors.Control
        Me.picXRayOn.Cursor = System.Windows.Forms.Cursors.Default
        Me.picXRayOn.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.picXRayOn.ForeColor = System.Drawing.SystemColors.ControlText
        Me.picXRayOn.Image = CType(resources.GetObject("picXRayOn.Image"), System.Drawing.Image)
        Me.picXRayOn.Location = New System.Drawing.Point(208, 56)
        Me.picXRayOn.Name = "picXRayOn"
        Me.picXRayOn.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.picXRayOn.Size = New System.Drawing.Size(32, 32)
        Me.picXRayOn.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize
        Me.picXRayOn.TabIndex = 36
        Me.picXRayOn.TabStop = False
        Me.picXRayOn.Visible = False
        '
        'tmrXRayOn
        '
        Me.tmrXRayOn.Interval = 1000
        '
        'tmrMonitor
        '
        Me.tmrMonitor.Interval = 1500
        '
        'tmrInterLock
        '
        Me.tmrInterLock.Interval = 4000
        '
        'fraSettings
        '
        Me.fraSettings.BackColor = System.Drawing.SystemColors.Control
        Me.fraSettings.Controls.Add(Me.cmdSetHVandCurrent)
        Me.fraSettings.Controls.Add(Me.txtCurrent)
        Me.fraSettings.Controls.Add(Me.txtHv)
        Me.fraSettings.Controls.Add(Me.cmdExit)
        Me.fraSettings.Controls.Add(Me.cmdHvOff)
        Me.fraSettings.Controls.Add(Me.cmdHvOn)
        Me.fraSettings.Controls.Add(Me.lblWattageOutOfRange)
        Me.fraSettings.Controls.Add(Me._lbllblMonitor_1)
        Me.fraSettings.Controls.Add(Me._lbllblMonitor_0)
        Me.fraSettings.Controls.Add(Me._lbluA_1)
        Me.fraSettings.Controls.Add(Me._lbluA_0)
        Me.fraSettings.Controls.Add(Me._lblkV_1)
        Me.fraSettings.Controls.Add(Me._lblkV_0)
        Me.fraSettings.Controls.Add(Me.lblCurrentMonitor)
        Me.fraSettings.Controls.Add(Me.lblHvMonitor)
        Me.fraSettings.Controls.Add(Me._lblMAX_2)
        Me.fraSettings.Controls.Add(Me._lblMAX_1)
        Me.fraSettings.Controls.Add(Me._lblMIN_2)
        Me.fraSettings.Controls.Add(Me._lblMIN_1)
        Me.fraSettings.Controls.Add(Me._lblMAX_0)
        Me.fraSettings.Controls.Add(Me._lblMIN_0)
        Me.fraSettings.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.fraSettings.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.fraSettings.ForeColor = System.Drawing.SystemColors.ControlText
        Me.fraSettings.Location = New System.Drawing.Point(8, 110)
        Me.fraSettings.Name = "fraSettings"
        Me.fraSettings.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.fraSettings.Size = New System.Drawing.Size(433, 145)
        Me.fraSettings.TabIndex = 11
        Me.fraSettings.TabStop = False
        Me.fraSettings.Text = "Set/Monitor High Voltage and Current"
        '
        'cmdSetHVandCurrent
        '
        Me.cmdSetHVandCurrent.BackColor = System.Drawing.SystemColors.Control
        Me.cmdSetHVandCurrent.Cursor = System.Windows.Forms.Cursors.Default
        Me.cmdSetHVandCurrent.Enabled = False
        Me.cmdSetHVandCurrent.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.cmdSetHVandCurrent.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.cmdSetHVandCurrent.ForeColor = System.Drawing.SystemColors.ControlText
        Me.cmdSetHVandCurrent.Location = New System.Drawing.Point(24, 40)
        Me.cmdSetHVandCurrent.Name = "cmdSetHVandCurrent"
        Me.cmdSetHVandCurrent.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.cmdSetHVandCurrent.Size = New System.Drawing.Size(97, 41)
        Me.cmdSetHVandCurrent.TabIndex = 31
        Me.cmdSetHVandCurrent.Text = "Set High Voltage and Current"
        '
        'txtCurrent
        '
        Me.txtCurrent.AcceptsReturn = True
        Me.txtCurrent.AutoSize = False
        Me.txtCurrent.BackColor = System.Drawing.SystemColors.Window
        Me.txtCurrent.Cursor = System.Windows.Forms.Cursors.IBeam
        Me.txtCurrent.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.txtCurrent.ForeColor = System.Drawing.SystemColors.WindowText
        Me.txtCurrent.Location = New System.Drawing.Point(128, 64)
        Me.txtCurrent.MaxLength = 0
        Me.txtCurrent.Name = "txtCurrent"
        Me.txtCurrent.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.txtCurrent.Size = New System.Drawing.Size(57, 19)
        Me.txtCurrent.TabIndex = 18
        Me.txtCurrent.Text = "15"
        '
        'txtHv
        '
        Me.txtHv.AcceptsReturn = True
        Me.txtHv.AutoSize = False
        Me.txtHv.BackColor = System.Drawing.SystemColors.Window
        Me.txtHv.Cursor = System.Windows.Forms.Cursors.IBeam
        Me.txtHv.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.txtHv.ForeColor = System.Drawing.SystemColors.WindowText
        Me.txtHv.Location = New System.Drawing.Point(128, 40)
        Me.txtHv.MaxLength = 0
        Me.txtHv.Name = "txtHv"
        Me.txtHv.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.txtHv.Size = New System.Drawing.Size(57, 19)
        Me.txtHv.TabIndex = 15
        Me.txtHv.Text = "15"
        '
        'cmdExit
        '
        Me.cmdExit.BackColor = System.Drawing.SystemColors.Control
        Me.cmdExit.Cursor = System.Windows.Forms.Cursors.Default
        Me.cmdExit.DialogResult = System.Windows.Forms.DialogResult.Cancel
        Me.cmdExit.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.cmdExit.ForeColor = System.Drawing.SystemColors.ControlText
        Me.cmdExit.Image = CType(resources.GetObject("cmdExit.Image"), System.Drawing.Image)
        Me.cmdExit.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.cmdExit.Location = New System.Drawing.Point(256, 92)
        Me.cmdExit.Name = "cmdExit"
        Me.cmdExit.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.cmdExit.Size = New System.Drawing.Size(169, 45)
        Me.cmdExit.TabIndex = 14
        Me.cmdExit.Text = "E&xit"
        '
        'cmdHvOff
        '
        Me.cmdHvOff.BackColor = System.Drawing.SystemColors.Control
        Me.cmdHvOff.Cursor = System.Windows.Forms.Cursors.Default
        Me.cmdHvOff.Enabled = False
        Me.cmdHvOff.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.cmdHvOff.ForeColor = System.Drawing.SystemColors.ControlText
        Me.cmdHvOff.Image = CType(resources.GetObject("cmdHvOff.Image"), System.Drawing.Image)
        Me.cmdHvOff.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.cmdHvOff.Location = New System.Drawing.Point(320, 12)
        Me.cmdHvOff.Name = "cmdHvOff"
        Me.cmdHvOff.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.cmdHvOff.Size = New System.Drawing.Size(105, 36)
        Me.cmdHvOff.TabIndex = 13
        Me.cmdHvOff.Text = "HV OFF"
        Me.cmdHvOff.TextAlign = System.Drawing.ContentAlignment.MiddleRight
        '
        'cmdHvOn
        '
        Me.cmdHvOn.BackColor = System.Drawing.SystemColors.Control
        Me.cmdHvOn.Cursor = System.Windows.Forms.Cursors.Default
        Me.cmdHvOn.Enabled = False
        Me.cmdHvOn.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.cmdHvOn.ForeColor = System.Drawing.SystemColors.ControlText
        Me.cmdHvOn.Image = CType(resources.GetObject("cmdHvOn.Image"), System.Drawing.Image)
        Me.cmdHvOn.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.cmdHvOn.Location = New System.Drawing.Point(320, 52)
        Me.cmdHvOn.Name = "cmdHvOn"
        Me.cmdHvOn.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.cmdHvOn.Size = New System.Drawing.Size(105, 36)
        Me.cmdHvOn.TabIndex = 12
        Me.cmdHvOn.Text = "HV ON"
        Me.cmdHvOn.TextAlign = System.Drawing.ContentAlignment.MiddleRight
        '
        'lblWattageOutOfRange
        '
        Me.lblWattageOutOfRange.BackColor = System.Drawing.Color.Transparent
        Me.lblWattageOutOfRange.Cursor = System.Windows.Forms.Cursors.Default
        Me.lblWattageOutOfRange.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.lblWattageOutOfRange.ForeColor = System.Drawing.Color.FromArgb(CType(0, Byte), CType(128, Byte), CType(128, Byte))
        Me.lblWattageOutOfRange.Location = New System.Drawing.Point(24, 16)
        Me.lblWattageOutOfRange.Name = "lblWattageOutOfRange"
        Me.lblWattageOutOfRange.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.lblWattageOutOfRange.Size = New System.Drawing.Size(161, 17)
        Me.lblWattageOutOfRange.TabIndex = 35
        Me.lblWattageOutOfRange.Text = ">> Wattage Out Of Range <<"
        Me.lblWattageOutOfRange.TextAlign = System.Drawing.ContentAlignment.TopCenter
        Me.lblWattageOutOfRange.Visible = False
        '
        '_lbllblMonitor_1
        '
        Me._lbllblMonitor_1.BackColor = System.Drawing.Color.Transparent
        Me._lbllblMonitor_1.Cursor = System.Windows.Forms.Cursors.Default
        Me._lbllblMonitor_1.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._lbllblMonitor_1.ForeColor = System.Drawing.SystemColors.ControlText
        Me.lbllblMonitor.SetIndex(Me._lbllblMonitor_1, CType(1, Short))
        Me._lbllblMonitor_1.Location = New System.Drawing.Point(8, 120)
        Me._lbllblMonitor_1.Name = "_lbllblMonitor_1"
        Me._lbllblMonitor_1.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._lbllblMonitor_1.Size = New System.Drawing.Size(113, 17)
        Me._lbllblMonitor_1.TabIndex = 30
        Me._lbllblMonitor_1.Text = "Current Monitor"
        Me._lbllblMonitor_1.TextAlign = System.Drawing.ContentAlignment.TopRight
        '
        '_lbllblMonitor_0
        '
        Me._lbllblMonitor_0.BackColor = System.Drawing.Color.Transparent
        Me._lbllblMonitor_0.Cursor = System.Windows.Forms.Cursors.Default
        Me._lbllblMonitor_0.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._lbllblMonitor_0.ForeColor = System.Drawing.SystemColors.ControlText
        Me.lbllblMonitor.SetIndex(Me._lbllblMonitor_0, CType(0, Short))
        Me._lbllblMonitor_0.Location = New System.Drawing.Point(8, 96)
        Me._lbllblMonitor_0.Name = "_lbllblMonitor_0"
        Me._lbllblMonitor_0.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._lbllblMonitor_0.Size = New System.Drawing.Size(113, 17)
        Me._lbllblMonitor_0.TabIndex = 29
        Me._lbllblMonitor_0.Text = "High Voltage Monitor"
        Me._lbllblMonitor_0.TextAlign = System.Drawing.ContentAlignment.TopRight
        '
        '_lbluA_1
        '
        Me._lbluA_1.BackColor = System.Drawing.Color.Transparent
        Me._lbluA_1.Cursor = System.Windows.Forms.Cursors.Default
        Me._lbluA_1.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._lbluA_1.ForeColor = System.Drawing.SystemColors.ControlText
        Me.lbluA.SetIndex(Me._lbluA_1, CType(1, Short))
        Me._lbluA_1.Location = New System.Drawing.Point(192, 120)
        Me._lbluA_1.Name = "_lbluA_1"
        Me._lbluA_1.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._lbluA_1.Size = New System.Drawing.Size(25, 17)
        Me._lbluA_1.TabIndex = 28
        Me._lbluA_1.Text = "uA"
        Me._lbluA_1.TextAlign = System.Drawing.ContentAlignment.TopCenter
        '
        '_lbluA_0
        '
        Me._lbluA_0.BackColor = System.Drawing.Color.Transparent
        Me._lbluA_0.Cursor = System.Windows.Forms.Cursors.Default
        Me._lbluA_0.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._lbluA_0.ForeColor = System.Drawing.SystemColors.ControlText
        Me.lbluA.SetIndex(Me._lbluA_0, CType(0, Short))
        Me._lbluA_0.Location = New System.Drawing.Point(272, 64)
        Me._lbluA_0.Name = "_lbluA_0"
        Me._lbluA_0.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._lbluA_0.Size = New System.Drawing.Size(25, 17)
        Me._lbluA_0.TabIndex = 27
        Me._lbluA_0.Text = "uA"
        Me._lbluA_0.TextAlign = System.Drawing.ContentAlignment.TopCenter
        '
        '_lblkV_1
        '
        Me._lblkV_1.BackColor = System.Drawing.Color.Transparent
        Me._lblkV_1.Cursor = System.Windows.Forms.Cursors.Default
        Me._lblkV_1.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._lblkV_1.ForeColor = System.Drawing.SystemColors.ControlText
        Me.lblkV.SetIndex(Me._lblkV_1, CType(1, Short))
        Me._lblkV_1.Location = New System.Drawing.Point(192, 96)
        Me._lblkV_1.Name = "_lblkV_1"
        Me._lblkV_1.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._lblkV_1.Size = New System.Drawing.Size(25, 17)
        Me._lblkV_1.TabIndex = 26
        Me._lblkV_1.Text = "kV"
        Me._lblkV_1.TextAlign = System.Drawing.ContentAlignment.TopCenter
        '
        '_lblkV_0
        '
        Me._lblkV_0.BackColor = System.Drawing.Color.Transparent
        Me._lblkV_0.Cursor = System.Windows.Forms.Cursors.Default
        Me._lblkV_0.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._lblkV_0.ForeColor = System.Drawing.SystemColors.ControlText
        Me.lblkV.SetIndex(Me._lblkV_0, CType(0, Short))
        Me._lblkV_0.Location = New System.Drawing.Point(272, 40)
        Me._lblkV_0.Name = "_lblkV_0"
        Me._lblkV_0.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._lblkV_0.Size = New System.Drawing.Size(25, 17)
        Me._lblkV_0.TabIndex = 25
        Me._lblkV_0.Text = "kV"
        Me._lblkV_0.TextAlign = System.Drawing.ContentAlignment.TopCenter
        '
        'lblCurrentMonitor
        '
        Me.lblCurrentMonitor.BackColor = System.Drawing.SystemColors.Control
        Me.lblCurrentMonitor.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.lblCurrentMonitor.Cursor = System.Windows.Forms.Cursors.Default
        Me.lblCurrentMonitor.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.lblCurrentMonitor.ForeColor = System.Drawing.SystemColors.ControlText
        Me.lblCurrentMonitor.Location = New System.Drawing.Point(128, 120)
        Me.lblCurrentMonitor.Name = "lblCurrentMonitor"
        Me.lblCurrentMonitor.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.lblCurrentMonitor.Size = New System.Drawing.Size(57, 17)
        Me.lblCurrentMonitor.TabIndex = 24
        Me.lblCurrentMonitor.Text = "0.0"
        '
        'lblHvMonitor
        '
        Me.lblHvMonitor.BackColor = System.Drawing.SystemColors.Control
        Me.lblHvMonitor.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.lblHvMonitor.Cursor = System.Windows.Forms.Cursors.Default
        Me.lblHvMonitor.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.lblHvMonitor.ForeColor = System.Drawing.SystemColors.ControlText
        Me.lblHvMonitor.Location = New System.Drawing.Point(128, 96)
        Me.lblHvMonitor.Name = "lblHvMonitor"
        Me.lblHvMonitor.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.lblHvMonitor.Size = New System.Drawing.Size(57, 17)
        Me.lblHvMonitor.TabIndex = 23
        Me.lblHvMonitor.Text = "0.0"
        '
        '_lblMAX_2
        '
        Me._lblMAX_2.BackColor = System.Drawing.Color.Transparent
        Me._lblMAX_2.Cursor = System.Windows.Forms.Cursors.Default
        Me._lblMAX_2.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._lblMAX_2.ForeColor = System.Drawing.SystemColors.ControlText
        Me.lblMAX.SetIndex(Me._lblMAX_2, CType(2, Short))
        Me._lblMAX_2.Location = New System.Drawing.Point(232, 64)
        Me._lblMAX_2.Name = "_lblMAX_2"
        Me._lblMAX_2.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._lblMAX_2.Size = New System.Drawing.Size(33, 17)
        Me._lblMAX_2.TabIndex = 22
        Me._lblMAX_2.Text = "200"
        Me._lblMAX_2.TextAlign = System.Drawing.ContentAlignment.TopCenter
        '
        '_lblMAX_1
        '
        Me._lblMAX_1.BackColor = System.Drawing.Color.Transparent
        Me._lblMAX_1.Cursor = System.Windows.Forms.Cursors.Default
        Me._lblMAX_1.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._lblMAX_1.ForeColor = System.Drawing.SystemColors.ControlText
        Me.lblMAX.SetIndex(Me._lblMAX_1, CType(1, Short))
        Me._lblMAX_1.Location = New System.Drawing.Point(232, 40)
        Me._lblMAX_1.Name = "_lblMAX_1"
        Me._lblMAX_1.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._lblMAX_1.Size = New System.Drawing.Size(33, 17)
        Me._lblMAX_1.TabIndex = 21
        Me._lblMAX_1.Text = "40"
        Me._lblMAX_1.TextAlign = System.Drawing.ContentAlignment.TopCenter
        '
        '_lblMIN_2
        '
        Me._lblMIN_2.BackColor = System.Drawing.Color.Transparent
        Me._lblMIN_2.Cursor = System.Windows.Forms.Cursors.Default
        Me._lblMIN_2.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._lblMIN_2.ForeColor = System.Drawing.SystemColors.ControlText
        Me.lblMIN.SetIndex(Me._lblMIN_2, CType(2, Short))
        Me._lblMIN_2.Location = New System.Drawing.Point(192, 64)
        Me._lblMIN_2.Name = "_lblMIN_2"
        Me._lblMIN_2.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._lblMIN_2.Size = New System.Drawing.Size(33, 17)
        Me._lblMIN_2.TabIndex = 20
        Me._lblMIN_2.Text = "5"
        Me._lblMIN_2.TextAlign = System.Drawing.ContentAlignment.TopCenter
        '
        '_lblMIN_1
        '
        Me._lblMIN_1.BackColor = System.Drawing.Color.Transparent
        Me._lblMIN_1.Cursor = System.Windows.Forms.Cursors.Default
        Me._lblMIN_1.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._lblMIN_1.ForeColor = System.Drawing.SystemColors.ControlText
        Me.lblMIN.SetIndex(Me._lblMIN_1, CType(1, Short))
        Me._lblMIN_1.Location = New System.Drawing.Point(192, 40)
        Me._lblMIN_1.Name = "_lblMIN_1"
        Me._lblMIN_1.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._lblMIN_1.Size = New System.Drawing.Size(33, 17)
        Me._lblMIN_1.TabIndex = 19
        Me._lblMIN_1.Text = "10"
        Me._lblMIN_1.TextAlign = System.Drawing.ContentAlignment.TopCenter
        '
        '_lblMAX_0
        '
        Me._lblMAX_0.BackColor = System.Drawing.Color.Transparent
        Me._lblMAX_0.Cursor = System.Windows.Forms.Cursors.Default
        Me._lblMAX_0.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._lblMAX_0.ForeColor = System.Drawing.SystemColors.ControlText
        Me.lblMAX.SetIndex(Me._lblMAX_0, CType(0, Short))
        Me._lblMAX_0.Location = New System.Drawing.Point(232, 16)
        Me._lblMAX_0.Name = "_lblMAX_0"
        Me._lblMAX_0.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._lblMAX_0.Size = New System.Drawing.Size(33, 17)
        Me._lblMAX_0.TabIndex = 17
        Me._lblMAX_0.Text = "MAX"
        Me._lblMAX_0.TextAlign = System.Drawing.ContentAlignment.TopCenter
        '
        '_lblMIN_0
        '
        Me._lblMIN_0.BackColor = System.Drawing.Color.Transparent
        Me._lblMIN_0.Cursor = System.Windows.Forms.Cursors.Default
        Me._lblMIN_0.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me._lblMIN_0.ForeColor = System.Drawing.SystemColors.ControlText
        Me.lblMIN.SetIndex(Me._lblMIN_0, CType(0, Short))
        Me._lblMIN_0.Location = New System.Drawing.Point(192, 16)
        Me._lblMIN_0.Name = "_lblMIN_0"
        Me._lblMIN_0.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me._lblMIN_0.Size = New System.Drawing.Size(33, 17)
        Me._lblMIN_0.TabIndex = 16
        Me._lblMIN_0.Text = "MIN"
        Me._lblMIN_0.TextAlign = System.Drawing.ContentAlignment.TopCenter
        '
        'cmdSTART_MINIX_CONTROLLER
        '
        Me.cmdSTART_MINIX_CONTROLLER.BackColor = System.Drawing.SystemColors.Control
        Me.cmdSTART_MINIX_CONTROLLER.Cursor = System.Windows.Forms.Cursors.Default
        Me.cmdSTART_MINIX_CONTROLLER.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.cmdSTART_MINIX_CONTROLLER.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.cmdSTART_MINIX_CONTROLLER.ForeColor = System.Drawing.SystemColors.ControlText
        Me.cmdSTART_MINIX_CONTROLLER.Location = New System.Drawing.Point(112, 8)
        Me.cmdSTART_MINIX_CONTROLLER.Name = "cmdSTART_MINIX_CONTROLLER"
        Me.cmdSTART_MINIX_CONTROLLER.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.cmdSTART_MINIX_CONTROLLER.Size = New System.Drawing.Size(217, 25)
        Me.cmdSTART_MINIX_CONTROLLER.TabIndex = 4
        Me.cmdSTART_MINIX_CONTROLLER.Text = "Start Amptek Mini-X"
        '
        'picIsoCurve
        '
        Me.picIsoCurve.BackColor = System.Drawing.SystemColors.Control
        Me.picIsoCurve.Cursor = System.Windows.Forms.Cursors.Default
        Me.picIsoCurve.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.picIsoCurve.ForeColor = System.Drawing.SystemColors.ControlText
        Me.picIsoCurve.Image = CType(resources.GetObject("picIsoCurve.Image"), System.Drawing.Image)
        Me.picIsoCurve.Location = New System.Drawing.Point(264, 256)
        Me.picIsoCurve.Name = "picIsoCurve"
        Me.picIsoCurve.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.picIsoCurve.Size = New System.Drawing.Size(172, 93)
        Me.picIsoCurve.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize
        Me.picIsoCurve.TabIndex = 3
        Me.picIsoCurve.TabStop = False
        '
        'picMiniXHw
        '
        Me.picMiniXHw.BackColor = System.Drawing.SystemColors.Control
        Me.picMiniXHw.Cursor = System.Windows.Forms.Cursors.Default
        Me.picMiniXHw.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.picMiniXHw.ForeColor = System.Drawing.SystemColors.ControlText
        Me.picMiniXHw.Image = CType(resources.GetObject("picMiniXHw.Image"), System.Drawing.Image)
        Me.picMiniXHw.Location = New System.Drawing.Point(8, 256)
        Me.picMiniXHw.Name = "picMiniXHw"
        Me.picMiniXHw.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.picMiniXHw.Size = New System.Drawing.Size(190, 93)
        Me.picMiniXHw.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize
        Me.picMiniXHw.TabIndex = 2
        Me.picMiniXHw.TabStop = False
        '
        'picMiniXLogo
        '
        Me.picMiniXLogo.BackColor = System.Drawing.SystemColors.Control
        Me.picMiniXLogo.Cursor = System.Windows.Forms.Cursors.Default
        Me.picMiniXLogo.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.picMiniXLogo.ForeColor = System.Drawing.SystemColors.ControlText
        Me.picMiniXLogo.Image = CType(resources.GetObject("picMiniXLogo.Image"), System.Drawing.Image)
        Me.picMiniXLogo.Location = New System.Drawing.Point(344, 7)
        Me.picMiniXLogo.Name = "picMiniXLogo"
        Me.picMiniXLogo.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.picMiniXLogo.Size = New System.Drawing.Size(98, 45)
        Me.picMiniXLogo.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize
        Me.picMiniXLogo.TabIndex = 1
        Me.picMiniXLogo.TabStop = False
        '
        'picAmptekLogo
        '
        Me.picAmptekLogo.BackColor = System.Drawing.SystemColors.Control
        Me.picAmptekLogo.Cursor = System.Windows.Forms.Cursors.Default
        Me.picAmptekLogo.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.picAmptekLogo.ForeColor = System.Drawing.SystemColors.ControlText
        Me.picAmptekLogo.Image = CType(resources.GetObject("picAmptekLogo.Image"), System.Drawing.Image)
        Me.picAmptekLogo.Location = New System.Drawing.Point(7, 7)
        Me.picAmptekLogo.Name = "picAmptekLogo"
        Me.picAmptekLogo.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.picAmptekLogo.Size = New System.Drawing.Size(92, 45)
        Me.picAmptekLogo.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize
        Me.picAmptekLogo.TabIndex = 0
        Me.picAmptekLogo.TabStop = False
        '
        'lblPower_mW
        '
        Me.lblPower_mW.BackColor = System.Drawing.Color.White
        Me.lblPower_mW.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.lblPower_mW.Cursor = System.Windows.Forms.Cursors.Default
        Me.lblPower_mW.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.lblPower_mW.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.lblPower_mW.ForeColor = System.Drawing.SystemColors.ControlText
        Me.lblPower_mW.Location = New System.Drawing.Point(208, 332)
        Me.lblPower_mW.Name = "lblPower_mW"
        Me.lblPower_mW.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.lblPower_mW.Size = New System.Drawing.Size(49, 17)
        Me.lblPower_mW.TabIndex = 8
        Me.lblPower_mW.TextAlign = System.Drawing.ContentAlignment.TopCenter
        '
        'lblXRayOn
        '
        Me.lblXRayOn.BackColor = System.Drawing.Color.Transparent
        Me.lblXRayOn.Cursor = System.Windows.Forms.Cursors.Default
        Me.lblXRayOn.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.lblXRayOn.ForeColor = System.Drawing.SystemColors.ControlText
        Me.lblXRayOn.Location = New System.Drawing.Point(192, 92)
        Me.lblXRayOn.Name = "lblXRayOn"
        Me.lblXRayOn.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.lblXRayOn.Size = New System.Drawing.Size(65, 17)
        Me.lblXRayOn.TabIndex = 37
        Me.lblXRayOn.Text = "X-RAY ON"
        Me.lblXRayOn.TextAlign = System.Drawing.ContentAlignment.TopCenter
        Me.lblXRayOn.Visible = False
        '
        'lblInterLock
        '
        Me.lblInterLock.BackColor = System.Drawing.Color.Transparent
        Me.lblInterLock.Cursor = System.Windows.Forms.Cursors.Default
        Me.lblInterLock.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.lblInterLock.ForeColor = System.Drawing.SystemColors.ControlText
        Me.lblInterLock.Location = New System.Drawing.Point(248, 72)
        Me.lblInterLock.Name = "lblInterLock"
        Me.lblInterLock.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.lblInterLock.Size = New System.Drawing.Size(137, 17)
        Me.lblInterLock.TabIndex = 34
        Me.lblInterLock.Text = "INTERLOCK RESTORED"
        Me.lblInterLock.TextAlign = System.Drawing.ContentAlignment.TopRight
        '
        'lblSerialNumber
        '
        Me.lblSerialNumber.BackColor = System.Drawing.Color.Transparent
        Me.lblSerialNumber.Cursor = System.Windows.Forms.Cursors.Default
        Me.lblSerialNumber.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.lblSerialNumber.ForeColor = System.Drawing.SystemColors.ControlText
        Me.lblSerialNumber.Location = New System.Drawing.Point(112, 35)
        Me.lblSerialNumber.Name = "lblSerialNumber"
        Me.lblSerialNumber.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.lblSerialNumber.Size = New System.Drawing.Size(217, 17)
        Me.lblSerialNumber.TabIndex = 10
        Me.lblSerialNumber.Text = "lblSerialNumber"
        Me.lblSerialNumber.TextAlign = System.Drawing.ContentAlignment.TopCenter
        '
        'lblBoardTemp
        '
        Me.lblBoardTemp.BackColor = System.Drawing.SystemColors.Control
        Me.lblBoardTemp.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.lblBoardTemp.Cursor = System.Windows.Forms.Cursors.Default
        Me.lblBoardTemp.FlatStyle = System.Windows.Forms.FlatStyle.System
        Me.lblBoardTemp.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.lblBoardTemp.ForeColor = System.Drawing.SystemColors.ControlText
        Me.lblBoardTemp.Location = New System.Drawing.Point(208, 285)
        Me.lblBoardTemp.Name = "lblBoardTemp"
        Me.lblBoardTemp.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.lblBoardTemp.Size = New System.Drawing.Size(49, 17)
        Me.lblBoardTemp.TabIndex = 9
        Me.lblBoardTemp.TextAlign = System.Drawing.ContentAlignment.TopCenter
        '
        'lbllblPower_mW
        '
        Me.lbllblPower_mW.BackColor = System.Drawing.Color.Transparent
        Me.lbllblPower_mW.Cursor = System.Windows.Forms.Cursors.Default
        Me.lbllblPower_mW.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.lbllblPower_mW.ForeColor = System.Drawing.SystemColors.ControlText
        Me.lbllblPower_mW.Location = New System.Drawing.Point(204, 304)
        Me.lbllblPower_mW.Name = "lbllblPower_mW"
        Me.lbllblPower_mW.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.lbllblPower_mW.Size = New System.Drawing.Size(57, 27)
        Me.lbllblPower_mW.TabIndex = 7
        Me.lbllblPower_mW.Text = "Power mW"
        Me.lbllblPower_mW.TextAlign = System.Drawing.ContentAlignment.TopCenter
        '
        'lbllblBoardTemp
        '
        Me.lbllblBoardTemp.BackColor = System.Drawing.Color.Transparent
        Me.lbllblBoardTemp.Cursor = System.Windows.Forms.Cursors.Default
        Me.lbllblBoardTemp.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.lbllblBoardTemp.ForeColor = System.Drawing.SystemColors.ControlText
        Me.lbllblBoardTemp.Location = New System.Drawing.Point(208, 256)
        Me.lbllblBoardTemp.Name = "lbllblBoardTemp"
        Me.lbllblBoardTemp.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.lbllblBoardTemp.Size = New System.Drawing.Size(49, 33)
        Me.lbllblBoardTemp.TabIndex = 6
        Me.lbllblBoardTemp.Text = "Board Temp.°C"
        Me.lbllblBoardTemp.TextAlign = System.Drawing.ContentAlignment.TopCenter
        '
        'lblStatus
        '
        Me.lblStatus.BackColor = System.Drawing.SystemColors.Control
        Me.lblStatus.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.lblStatus.Cursor = System.Windows.Forms.Cursors.Default
        Me.lblStatus.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.lblStatus.ForeColor = System.Drawing.SystemColors.ControlText
        Me.lblStatus.Location = New System.Drawing.Point(8, 352)
        Me.lblStatus.Name = "lblStatus"
        Me.lblStatus.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.lblStatus.Size = New System.Drawing.Size(433, 25)
        Me.lblStatus.TabIndex = 5
        Me.lblStatus.TextAlign = System.Drawing.ContentAlignment.TopCenter
        '
        'picIsoCurve50kV
        '
        Me.picIsoCurve50kV.BackColor = System.Drawing.SystemColors.Control
        Me.picIsoCurve50kV.Cursor = System.Windows.Forms.Cursors.Default
        Me.picIsoCurve50kV.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.picIsoCurve50kV.ForeColor = System.Drawing.SystemColors.ControlText
        Me.picIsoCurve50kV.Image = CType(resources.GetObject("picIsoCurve50kV.Image"), System.Drawing.Image)
        Me.picIsoCurve50kV.Location = New System.Drawing.Point(264, 256)
        Me.picIsoCurve50kV.Name = "picIsoCurve50kV"
        Me.picIsoCurve50kV.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.picIsoCurve50kV.Size = New System.Drawing.Size(172, 93)
        Me.picIsoCurve50kV.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize
        Me.picIsoCurve50kV.TabIndex = 38
        Me.picIsoCurve50kV.TabStop = False
        Me.picIsoCurve50kV.Visible = False
        '
        'frmMiniX
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
        Me.BackColor = System.Drawing.SystemColors.Control
        Me.CancelButton = Me.cmdExit
        Me.ClientSize = New System.Drawing.Size(451, 386)
        Me.Controls.Add(Me.picIsoCurve50kV)
        Me.Controls.Add(Me.picXRayOn)
        Me.Controls.Add(Me._picInterLock_1)
        Me.Controls.Add(Me._picInterLock_0)
        Me.Controls.Add(Me.fraSettings)
        Me.Controls.Add(Me.cmdSTART_MINIX_CONTROLLER)
        Me.Controls.Add(Me.picIsoCurve)
        Me.Controls.Add(Me.picMiniXHw)
        Me.Controls.Add(Me.picMiniXLogo)
        Me.Controls.Add(Me.picAmptekLogo)
        Me.Controls.Add(Me.lblPower_mW)
        Me.Controls.Add(Me.lblXRayOn)
        Me.Controls.Add(Me.lblInterLock)
        Me.Controls.Add(Me.lblSerialNumber)
        Me.Controls.Add(Me.lblBoardTemp)
        Me.Controls.Add(Me.lbllblPower_mW)
        Me.Controls.Add(Me.lbllblBoardTemp)
        Me.Controls.Add(Me.lblStatus)
        Me.Cursor = System.Windows.Forms.Cursors.Default
        Me.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.KeyPreview = True
        Me.Location = New System.Drawing.Point(4, 30)
        Me.Name = "frmMiniX"
        Me.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.Text = "Mini-X Controller"
        Me.fraSettings.ResumeLayout(False)
        CType(Me.lblMAX, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.lblMIN, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.lblkV, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.lbllblMonitor, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.lbluA, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.picInterLock, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)

    End Sub
#End Region
#Region "Upgrade Support "
    Private Shared m_vb6FormDefInstance As frmMiniX
    Private Shared m_InitializingDefInstance As Boolean
    Public Shared Property DefInstance() As frmMiniX
        Get
            If m_vb6FormDefInstance Is Nothing OrElse m_vb6FormDefInstance.IsDisposed Then
                m_InitializingDefInstance = True
                m_vb6FormDefInstance = New frmMiniX
                m_InitializingDefInstance = False
            End If
            DefInstance = m_vb6FormDefInstance
        End Get
        Set(ByVal Value As frmMiniX)
            m_vb6FormDefInstance = Value
        End Set
    End Property
#End Region

    Private Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Integer)

    Private indInterlockStatus As Boolean
    Private indDisableMonitorCmds As Boolean

    Private Sub cmdExit_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdExit.Click
        On Error Resume Next
        If (CBool(isMiniXDlg())) Then
            tmrMonitor.Enabled = False
            SendMiniXCommand(CByte(modMiniX.MiniX_Commands.mxcExit))
            Sleep(100)
        End If
        Me.Close()
    End Sub

    Private Sub cmdHvOff_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdHvOff.Click
        indDisableMonitorCmds = True
        cmdHvOff.Enabled = False
        If (CBool(isMiniXDlg())) Then
            SendMiniXCommand(CByte(modMiniX.MiniX_Commands.mxcHVOff))
            Sleep(100)
        End If
        indDisableMonitorCmds = False
    End Sub

    Private Sub cmdHvOn_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdHvOn.Click
        Dim msgResponse As Short
        indDisableMonitorCmds = True
        cmdHvOn.Enabled = False
        If (CBool(isMiniXDlg())) Then
            msgResponse = MsgBox("Turn X-RAY High Voltage ON?", MsgBoxStyle.YesNo + MsgBoxStyle.Question + MsgBoxStyle.SystemModal, "MiniX")
            If msgResponse = MsgBoxResult.Yes Then
                SendMiniXCommand(CByte(modMiniX.MiniX_Commands.mxcHVOn))
                Sleep(100)
            End If
        End If
        indDisableMonitorCmds = False
    End Sub

    Private Sub cmdSetHVandCurrent_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdSetHVandCurrent.Click
        Dim dblValue As Double
        Dim MiniXSettings As MiniX_Settings
        indDisableMonitorCmds = True
        cmdSetHVandCurrent.Enabled = False
        cmdHvOn.Enabled = False
        cmdHvOff.Enabled = False
        If (CBool(isMiniXDlg())) Then
            dblValue = CDbl(txtHv.Text) 'sent hv value
            SetMiniXHV(dblValue)
            Sleep(100)
            dblValue = CDbl(txtCurrent.Text) 'send current value
            SetMiniXCurrent(dblValue)
            Sleep(100)
            SendMiniXCommand(CByte(modMiniX.MiniX_Commands.mxcSetHVandCurrent))
            Sleep(1000)
            ReadMiniXSettings(MiniXSettings) 'read the corrected values back
            txtHv.Text = CStr(MiniXSettings.HighVoltage_kV)
            txtCurrent.Text = CStr(MiniXSettings.Current_uA)
        End If
        indDisableMonitorCmds = False
    End Sub

    Private Sub cmdSTART_MINIX_CONTROLLER_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdSTART_MINIX_CONTROLLER.Click
        Dim MiniXSerialNumber As Integer
        Dim strSerialNumber As String

        '==== MiniXSerialNumber >= 10000 and < 1118880 40kV Tube ====
        '==== MiniXSerialNumber >= 1118880 50kV Tube ====
        picIsoCurve.Visible = True
        picIsoCurve50kV().Visible = False
        _lblMAX_1.Text = "40"

        indDisableMonitorCmds = True
        cmdSTART_MINIX_CONTROLLER.Enabled = False
        If (CBool(isMiniXDlg())) Then
            SendMiniXCommand(CByte(modMiniX.MiniX_Commands.mxcStartMiniX))
            Sleep(100)
            tmrMonitor.Enabled = True
        End If
        If (CBool(isMiniXDlg())) Then
            Sleep(100)
            MiniXSerialNumber = ReadMiniXSerialNumber()
            If (MiniXSerialNumber > 0) Then
                strSerialNumber = VB6.Format(MiniXSerialNumber, "00000000")
                strSerialNumber = "Mini-X Serial Number " & strSerialNumber
                lblSerialNumber.Text = strSerialNumber
                If (MiniXSerialNumber >= 1118880) Then  '50kV Tube
                    picIsoCurve.Visible = False
                    picIsoCurve50kV.Visible = True
                    _lblMAX_1.Text = "50"
                End If
            Else
                lblSerialNumber.Text = ""
            End If
        End If
        indDisableMonitorCmds = False
    End Sub

    Private Sub frmMiniX_KeyDown(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.KeyEventArgs) Handles MyBase.KeyDown
        Dim KeyCode As Short = eventArgs.KeyCode
        Dim Shift As Short = eventArgs.KeyData \ &H10000
        Dim AltDown As Boolean
        AltDown = CBool((Shift And VB6.ShiftConstants.AltMask) > 0)
        If (AltDown And (KeyCode = System.Windows.Forms.Keys.X)) Then
            cmdExit_Click(cmdExit, New System.EventArgs)
        End If
    End Sub

    Private Sub frmMiniX_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load
        Dim MiniXSettings As MiniX_Settings
        lblSerialNumber.Text = ""
        lblInterLock.Text = ""
        If (Not CBool(isMiniXDlg())) Then
            OpenMiniX()
            Sleep(100)
        End If
        If (CBool(isMiniXDlg())) Then
            ReadMiniXSettings(MiniXSettings)
            txtHv.Text = CStr(MiniXSettings.HighVoltage_kV)
            txtCurrent.Text = CStr(MiniXSettings.Current_uA)
        End If
    End Sub

    Private Sub frmMiniX_Closing(ByVal eventSender As System.Object, ByVal eventArgs As System.ComponentModel.CancelEventArgs) Handles MyBase.Closing
        Dim Cancel As Short = eventArgs.Cancel
        On Error Resume Next
        If (CBool(isMiniXDlg())) Then
            CloseMiniX()
            Sleep(100)
        End If
        eventArgs.Cancel = Cancel
    End Sub

    Private Sub frmMiniX_Closed(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Closed
        'Me.Close()
    End Sub

    Private Sub tmrInterLock_Tick(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles tmrInterLock.Tick
        picInterLock(0).Visible = False
        picInterLock(1).Visible = False
        lblInterLock.Text = ""
    End Sub

    Private Sub UpdateMonitor()
        Dim MiniXMonitor As MiniX_Monitor
        Dim strMonitor As String
        Dim indOldInterlockStatus As Boolean

        If (CBool(isMiniXDlg())) Then
            ReadMiniXMonitor(MiniXMonitor)
            Sleep(100)
            If (MiniXMonitor.mxmRefreshed) Then 'test if the status was updated
                If (MiniXMonitor.mxmReserved = 123.456) Then 'reserved but always 123.456
                    lblHvMonitor.Text = VB6.Format(MiniXMonitor.mxmHighVoltage_kV, "0.0")
                    lblCurrentMonitor.Text = VB6.Format(MiniXMonitor.mxmCurrent_uA, "0.0")
                    lblPower_mW.Text = VB6.Format(MiniXMonitor.mxmPower_mW, "0")
                    lblBoardTemp.Text = VB6.Format(MiniXMonitor.mxmTemperatureC, "0")
                    lblStatus.Text = GetMiniXStatusString(MiniXMonitor.mxmStatusInd)
                    indOldInterlockStatus = indInterlockStatus
                    indInterlockStatus = CBool(MiniXMonitor.mxmInterLock)
                    If (indOldInterlockStatus <> indInterlockStatus) Then
                        If (indInterlockStatus) Then
                            picInterLock(0).Visible = False
                            picInterLock(1).Visible = True
                            lblInterLock.Text = ToolTip1.GetToolTip(picInterLock(1))
                            tmrInterLock.Enabled = True
                        Else
                            picInterLock(0).Visible = True
                            picInterLock(1).Visible = False
                            lblInterLock.Text = ToolTip1.GetToolTip(picInterLock(0))
                            tmrInterLock.Enabled = False
                        End If
                    End If
                    EnableMiniX_Commands(MiniXMonitor.mxmEnabledCmds)
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

    Private Sub EnableMiniX_Commands(ByRef mxmEnabledCmds As Byte)
        Dim indStartMiniX As Boolean
        Dim indHVOn As Boolean
        Dim indHVOff As Boolean
        Dim indSetHVandCurrent As Boolean

        If (Not indDisableMonitorCmds) Then
            indStartMiniX = CBool(mxmEnabledCmds And modMiniX.MiniX_Commands.mxcStartMiniX)
            indHVOn = CBool(mxmEnabledCmds And modMiniX.MiniX_Commands.mxcHVOn)
            indHVOff = CBool(mxmEnabledCmds And modMiniX.MiniX_Commands.mxcHVOff)
            indSetHVandCurrent = CBool(mxmEnabledCmds And modMiniX.MiniX_Commands.mxcSetHVandCurrent)
            cmdSTART_MINIX_CONTROLLER.Enabled = indStartMiniX
            cmdHvOn.Enabled = indHVOn
            cmdHvOff.Enabled = indHVOff
            cmdSetHVandCurrent.Enabled = indSetHVandCurrent
        End If
    End Sub

    Private Sub tmrMonitor_Tick(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles tmrMonitor.Tick
        UpdateMonitor()
    End Sub

    Private Sub tmrXRayOn_Tick(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles tmrXRayOn.Tick
        lblXRayOn.Visible = True
        If (picXRayOn.Visible) Then
            picXRayOn.Visible = False
        Else
            picXRayOn.Visible = True
        End If
    End Sub
End Class
