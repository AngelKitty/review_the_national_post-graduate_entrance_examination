VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox Text2 
      Height          =   615
      Left            =   2220
      TabIndex        =   2
      Top             =   1380
      Width           =   1935
   End
   Begin VB.CommandButton Command1 
      Caption         =   "注册"
      Height          =   735
      Left            =   180
      TabIndex        =   1
      Top             =   2220
      Width           =   1815
   End
   Begin VB.TextBox Text1 
      Height          =   615
      Left            =   2220
      TabIndex        =   0
      Top             =   480
      Width           =   1935
   End
   Begin VB.Label Label3 
      Height          =   615
      Left            =   2340
      TabIndex        =   5
      Top             =   2280
      Width           =   1815
   End
   Begin VB.Label Label2 
      Caption         =   "注册码"
      Height          =   375
      Left            =   180
      TabIndex        =   4
      Top             =   1500
      Width           =   1755
   End
   Begin VB.Label Label1 
      Caption         =   "用户名"
      Height          =   375
      Left            =   180
      TabIndex        =   3
      Top             =   660
      Width           =   1695
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Command1_Click()
Dim name As String
Dim code As String
Dim sum As Long, i As Integer
name = Text1.Text
code = Text2.Text

If (Len(name) < 9 And Len(name) > 11) Or Len(code) < 9 Then
 MsgBox "错了！重新来吧"
Else:
For i = 1 To Len(name)
sum = sum + AscB(Mid(name, i)) * 4 + 18
Next i
sum = sum + 135790000
If sum = (CLng(Text2.Text) + 24) Then Label3.Caption = "高手，有空来坐坐" Else MsgBox "错了！重新来吧"


End If


End Sub

