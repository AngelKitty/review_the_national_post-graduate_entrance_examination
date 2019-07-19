; MASM32 antiRing3Debugger example 
; coded by ap0x
; Reversing Labs: http://ap0x.headcoders.net

ASSUME FS:NOTHING
PUSH offset _SehExit
PUSH DWORD PTR FS:[0]
MOV FS:[0],ESP

; Get NtGlobalFlag<- 这里ap0x出现bug了,这里获取的是PEB
MOV EAX,DWORD PTR FS:[30h]

; Get LDR_MODULE
MOV EAX,DWORD PTR[EAX+12]

; The trick is here ;) If ring3 debugger is present memory will be allocated
; and it will contain 0xFEEEFEEE bytes at the end of alloc. This will only
; happen if ring3 debugger is present!
; If there is no debugger SEH will fire and take control.

; Note: This code works only on NT systems!

_loop:
INC EAX
CMP DWORD PTR[EAX],0FEEEFEEEh
JNE _loop
DEC [Tries]
JNE _loop

PUSH 30h
PUSH offset DbgFoundTitle
PUSH offset DbgFoundText
PUSH 0
CALL MessageBox
PUSH 0
CALL ExitProcess
RET
_Exit:
PUSH 40h
PUSH offset DbgNotFoundTitle
PUSH offset DbgNotFoundText
PUSH 0
CALL MessageBox
PUSH 0
CALL ExitProcess
RET

_SehExit:
POP FS:[0]
ADD ESP,4
JMP _Exit
