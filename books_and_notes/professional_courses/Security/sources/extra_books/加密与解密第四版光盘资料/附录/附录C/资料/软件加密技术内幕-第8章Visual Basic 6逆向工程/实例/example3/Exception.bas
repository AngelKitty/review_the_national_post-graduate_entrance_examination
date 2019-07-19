Attribute VB_Name = "Module1"
Option Explicit

Public Const EXCEPTION_MAXIMUM_PARAMETERS = 15

' -----------------------------
' Exception-Handling Structures
' -----------------------------
Type EXCEPTION_POINTERS
     pExceptionRecord As Long       'pointer to an EXCEPTION_RECORD struct
     pContextRecord As Long          ' pointer to a CONTEXT struct
End Type

Type EXCEPTION_RECORD
     ExceptionCode As Long
     ExceptionFlags As Long
     pExceptionRecord As Long    ' Pointer to an EXCEPTION_RECORD structure
     ExceptionAddress As Long
     NumberParameters As Long
     ExceptionInformation(EXCEPTION_MAXIMUM_PARAMETERS) As Long
End Type

' ----------------------------
' Exception-Handling Functions
' ----------------------------
' Set lpTopLevelExceptionFilter parameter to AddressOf <New Handler>, or use 0 to restore default
' Returns address of previous exception handler
Declare Function SetUnhandledExceptionFilter Lib "kernel32" ( _
   ByVal lpTopLevelExceptionFilter As Long _
) As Long

Declare Function GetExceptionCode Lib "kernel32" () As Long

' Returns a pointer to an EXCEPTION_POINTERS structure
Declare Function GetExceptionInformation Lib "kernel32" () As Long

' Not used in this example
Declare Function RaiseException Lib "kernel32" ( _
   dwExceptionCode As Long, _
   dwExceptionFlags As Long, _
   nNumberOfArguments As Long, _
   lpArguments As Long _
   ) As Long

Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" ( _
   lpvDest As Any, lpvSource As Any, ByVal cbCopy As Long)


Public Function GetException(vValue As Variant) As String

' Function returns name of constant for given value.

Dim sName As String

Select Case vValue

   Case &HC0000005
      sName = "EXCEPTION_ACCESS_VIOLATION"
   Case &HC000008C
      sName = "EXCEPTION_ARRAY_BOUNDS_EXCEEDED"
   Case &H80000003
      sName = "EXCEPTION_BREAKPOINT"
   Case -1
      sName = "EXCEPTION_CONTINUE_EXECUTION"
   Case 0
      sName = "EXCEPTION_CONTINUE_SEARCH"
   Case &H80000002
      sName = "EXCEPTION_DATATYPE_MISALIGNMENT"
   Case 1
      sName = "EXCEPTION_DEBUG_EVENT"
   Case 1
      sName = "EXCEPTION_EXECUTE_HANDLER"
   Case &HC000008D
      sName = "EXCEPTION_FLT_DENORMAL_OPERAND"
   Case &HC000008E
      sName = "EXCEPTION_FLT_DIVIDE_BY_ZERO"
   Case &HC000008F
      sName = "EXCEPTION_FLT_INEXACT_RESULT"
   Case &HC0000090
      sName = "EXCEPTION_FLT_INVALID_OPERATION"
   Case &HC0000091
      sName = "EXCEPTION_FLT_OVERFLOW"
   Case &HC0000092
      sName = "EXCEPTION_FLT_STACK_CHECK"
   Case &HC0000093
      sName = "EXCEPTION_FLT_UNDERFLOW"
   Case &H80000001
      sName = "EXCEPTION_GUARD_PAGE"
   Case &HC000001D
      sName = "EXCEPTION_ILLEGAL_INSTRUCTION"
   Case &HC0000006
      sName = "EXCEPTION_IN_PAGE_ERROR"
   Case &HC0000094
      sName = "EXCEPTION_INT_DIVIDE_BY_ZERO"
   Case &HC0000095
      sName = "EXCEPTION_INT_OVERFLOW"
   Case &HC0000026
      sName = "EXCEPTION_INVALID_DISPOSITION"
   Case &HC0000008
      sName = "EXCEPTION_INVALID_HANDLE"
   Case 15
      sName = "EXCEPTION_MAXIMUM_PARAMETERS"
   Case &HC0000025
      sName = "EXCEPTION_NONCONTINUABLE_EXCEPTION"
   Case &HC0000096
      sName = "EXCEPTION_PRIV_INSTRUCTION"
   Case &H80000004
      sName = "EXCEPTION_SINGLE_STEP"
   Case &HC00000FD
      sName = "EXCEPTION_STACK_OVERFLOW"
   Case Else
      sName = "<invalid>"

End Select

GetException = sName

End Function

' This function receives the address of an EXCEPTION_POINTERS structure in its parameter
Function NewExceptionHandler(ByRef lpExceptionPointers As EXCEPTION_POINTERS) As Long

' No need to return a value since Err.Raise will alter execution flow

Dim er As EXCEPTION_RECORD
Dim sError As String

' Make a copy of the exception record portion
' of the passed-in EXCEPTION_POINTERS structure
CopyMemory er, ByVal lpExceptionPointers.pExceptionRecord, Len(er)

' Set up error description string
Do
   sError = GetException(er.ExceptionCode)
   ' Special treatment for access violation -- get addresses
   If sError = "EXCEPTION_ACCESS_VIOLATION" Then
      sError = sError & " - Instr @ &H" & Hex(er.ExceptionAddress) _
         & " tried illegally to " _
         & IIf(er.ExceptionInformation(0) = 0, "read from address", "write to address") _
         & " &H" & Hex(er.ExceptionInformation(1))
   End If
   
   ' Check for nested error
   If er.pExceptionRecord = 0 Then Exit Do
   
   ' Nested error exists
   ' Replace this er by the nested er
   CopyMemory er, ByVal er.pExceptionRecord, Len(er)
   
   ' New line for next error
   sError = sError & vbCrLf
Loop
   
' Raise an error to go up the call stack, passing the externally generated error!
Err.Raise 1000, "NewExceptionHandler", sError

End Function
