VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   2025
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   2025
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command1 
      Caption         =   "SEH   DEMO"
      Height          =   615
      Left            =   660
      TabIndex        =   0
      Top             =   600
      Width           =   3315
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Declare Sub DebugBreak Lib "kernel32" ()

Private Sub Command1_Click()
On Error GoTo ERR_RaiseException
DebugBreak
DebugBreak
Exit Sub

ERR_RaiseException:
   MsgBox "No tracer found!"
End Sub

Sub SetHandler()
SetUnhandledExceptionFilter AddressOf NewExceptionHandler
End Sub

Sub RestoreHandler()
SetUnhandledExceptionFilter 0
End Sub

Private Sub Form_Load()
SetHandler
End Sub

Private Sub Form_Unload(Cancel As Integer)
RestoreHandler
End Sub
