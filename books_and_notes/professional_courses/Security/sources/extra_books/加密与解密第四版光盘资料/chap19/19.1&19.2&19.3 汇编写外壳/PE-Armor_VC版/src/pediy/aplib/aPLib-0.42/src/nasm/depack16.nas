;;
;; aPLib compression library  -  the smaller the better :)
;;
;; NASM 16bit assembler depacker (only for use in assembly code)
;;
;; Copyright (c) 1998-2004 by Joergen Ibsen / Jibz
;; All Rights Reserved
;;
;; http://www.ibsensoftware.com/
;;
;; -> 16bit by Metalbrain (metalbrain_coder@gmx.net)
;;

;This code is thought to be copypasted directly in your code, at least that's
;the way I work (I'm not used to libraries, macros, C calling protocols, ...)

;
;IN:
;esi= inbuffer  (source)       <-MUST POINT RESERVED MEMORY INSIDE FIRST MEG!
;edi= outbuffer (destination)  <-MUST POINT RESERVED MEMORY INSIDE FIRST MEG!
;OUT:
;edi= uncompressed lenght
;
;It uses a lot of registers: EAX, BX, ECX, DX, EDI, ESI, EBP, ES
;to preserve them, push/pop where indicated

_aP_depack_asm
                ;Here you can PUSH the registers you're interested to preserve
                push    edi
                cld             ;Optional, remove if you're sure of DF=0
                mov     dl,128
literal         call    getesi
putedi_nexttag  call    putedi
                mov     dh,1
                jmp     short nexttag    ;Optional: Instead of unconditional
                                         ; jump, check limit on edi or esi,
                                         ; jump if no error and insert here
                                         ; error code. Limits on edi or esi
                                         ; can also be tested and handled in
                                         ; putedi or getesi.
normalcodepair  xchg    ax,cx
                dec     ax
                shl     eax,8
                call    getesi
                call    getgamma
                cmp     eax,32000
                jae     domatch_with_2inc
                cmp     ah,5
                jae     domatch_with_inc
                cmp     ax,byte 127
                ja      domatch_new_lastpos
domatch_with_2inc
                inc     ecx
domatch_with_inc
                inc     ecx
domatch_new_lastpos
                xchg    eax,ebp
domatch_lastpos mov     eax,ebp
                mov     dh,0
domatch
                push    esi
                mov     esi,edi
                sub     esi,eax
repmovsb        call    getesi
                call    putedi
                loop    repmovsb,ecx
                pop     esi
nexttag         call    getbit
                jnc     literal
                xor     ecx,ecx
                cbw
                call    getbit
                jnc     codepair
                call    getbit
                jnc     shortmatch
                mov     dh,1
                inc     cx
                mov     al,16
getmorebits     call    getbit
                adc     al,al
                jnc     getmorebits
                jnz     domatch
                jmp     short putedi_nexttag
codepair        call    getgamma
                shr     dh,1
                sbb     cx,byte 1
                jnz     normalcodepair

                push    word domatch_lastpos
getgamma        inc     cx
getgammaloop    call    getbit
                adc     ecx,ecx
                call    getbit
                jc      getgammaloop
                ret
shortmatch      call    getesi
                shr     ax,1
                jz      donedepacking
                adc     cx,cx
                jmp     short domatch_with_2inc
getbit          add     dl,dl
                jnz     stillbitsleft
                xchg    ax,dx
                call    getesi
                xchg    ax,dx
                stc
                adc     dl,dl
stillbitsleft   ret
donedepacking   pop     esi
                sub     edi,esi
                ;Here you can POP your registers back
                ret             ;You can quit this ret and use the routine
                                ; inline in your code

;
; This routines deal with 32 bit access through [esi] and [edi]
;
; Must be near-accesible from _aP_depack_asm
;

getesi          push    esi
                pop     bx
                pop     bx
                ror     bx,4
                mov     es,bx
                mov     al,[es:si]
                inc     esi
                ret
putedi          push    edi
                pop     bx
                pop     bx
                ror     bx,4
                mov     es,bx
                mov     [es:di],al
                inc     edi
                xor     eax,eax
                ret
