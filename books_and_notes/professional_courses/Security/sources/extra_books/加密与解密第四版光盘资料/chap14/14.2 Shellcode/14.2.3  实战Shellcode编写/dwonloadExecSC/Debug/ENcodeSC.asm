; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい?
    include \masm32\include\masm32rt.inc
; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい?

comment * -----------------------------------------------------
                     Build this console app with
                  'MAKEIT.BAT' on the PROJECT menu.
        ----------------------------------------------------- *

    .data?
      sc_start      dd ?
      sc_end        dd ?
      sc_len        dd ?
      out_handle    dd ?
      out_buff      dd ?
      dwsize        dd ?
    .data
      jmpLength1    dd 0
      jmpLength2    dd 0
      sc_out db 'EnCodeSC.bin',0
      exelen dd 1000h
      NN	MACRO	n;
      REPT	n
		NOP
			
	ENDM
ENDM

    .code

start:
   
; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい?

    call main
    inkey
    exit

; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい?

main proc


jmp scEnd;
scStart:
                assume  fs:nothing
			cld
		      call    SC_003B00AC
SC_003B00AC:
			pop     edi                              ; 003B00AC
			;sub     edi, 0ACh
			add     edi, 24h
			mov     ecx, 1875fh
SC_003B0101:
			add     byte ptr [edi], 6dh
			xor     byte ptr [edi], 8Ah
			sub     byte ptr [edi], 53h
			inc     edi
			dec     ecx
			jnz     SC_003B0101
			call    SC_003B01E0
SC_003B01E0:
			pop     eax
                  pop     eax
			jmp     SC_003B0219
                  nop
                  nop
                  nop
                  nop
                  nop
SC_003B0219:	          
                
scEnd:
mov sc_start,scStart
mov sc_end, scEnd
mov ebx,sc_end
sub ebx,sc_start
sub ebx, 5
mov sc_len,ebx

invoke CreateFile,offset sc_out,40000000h,3,0,2,80h,0
mov out_handle,eax

lea ebx,scStart
mov out_buff,ebx
invoke WriteFile,out_handle,out_buff,sc_len,addr dwsize,0

invoke CloseHandle,out_handle
ret

main endp

; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい?

end start
