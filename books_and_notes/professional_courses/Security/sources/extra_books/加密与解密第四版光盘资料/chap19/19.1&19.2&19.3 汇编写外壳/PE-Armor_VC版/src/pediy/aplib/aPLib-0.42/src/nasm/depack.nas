;;
;; aPLib compression library  -  the smaller the better :)
;;
;; NASM assembler depacker
;;
;; Copyright (c) 1998-2004 by Joergen Ibsen / Jibz
;; All Rights Reserved
;;
;; http://www.ibsensoftware.com/
;;
;; -> NASM by Archee/CoNTRACT (soltesz@hotmail.com)
;;

bits 32

%include "nasmlcm.inc"

section lcmtext

lcmglobal aP_depack_asm,8

lcmexport aP_depack_asm,8

; =============================================================

lcmlabel aP_depack_asm,8
    ; aP_depack_asm(const void *source, void *destination);

    _ret$  equ 7*4
    _src$  equ 8*4 + 4
    _dst$  equ 8*4 + 8

    pushad

    mov    esi, [esp + _src$] ; C calling convention
    mov    edi, [esp + _dst$]

    cld
    mov    dl, 80h
    xor    ebx,ebx

literal:
    movsb
    mov    bl, 2
nexttag:
    call   getbit
    jnc    short literal

    xor    ecx, ecx
    call   getbit
    jnc    short codepair
    xor    eax, eax
    call   getbit
    jnc    short shortmatch
    mov    bl, 2
    inc    ecx
    mov    al, 10h
  .getmorebits:
    call   getbit
    adc    al, al
    jnc    short .getmorebits
    jnz    short domatch
    stosb
    jmp    short nexttag
codepair:
    call   getgamma_no_ecx
    sub    ecx, ebx
    jnz    short normalcodepair
    call   getgamma
    jmp    short domatch_lastpos

shortmatch:
    lodsb
    shr    eax, 1
    jz     short donedepacking
    adc    ecx, ecx
    jmp    short domatch_with_2inc

normalcodepair:
    xchg   eax, ecx
    dec    eax
    shl    eax, 8
    lodsb
    call   getgamma

    cmp    eax, 32000
    jae    short domatch_with_2inc
    cmp    ah, 5
    jae    short domatch_with_inc
    cmp    eax, BYTE 7fh
    ja     short domatch_new_lastpos

domatch_with_2inc:
    inc    ecx

domatch_with_inc:
    inc    ecx

domatch_new_lastpos:
    xchg   eax, ebp
domatch_lastpos:
    mov    eax, ebp

    mov    bl, 1

domatch:
    push   esi
    mov    esi, edi
    sub    esi, eax
    rep    movsb
    pop    esi
    jmp    short nexttag

getbit:
    add    dl, dl
    jnz    short .stillbitsleft
    mov    dl, [esi]
    inc    esi
    adc    dl, dl
  .stillbitsleft:
    ret

getgamma:
    xor    ecx, ecx
getgamma_no_ecx:
    inc    ecx
  .getgammaloop:
    call   getbit
    adc    ecx, ecx
    call   getbit
    jc     short .getgammaloop
    ret

donedepacking:
    sub    edi, [esp + _dst$]
    mov    [esp + _ret$], edi ; return unpacked length in eax

    popad
    lcmret 8

; =============================================================

%ifdef LCM_OBJ
  section lcmdata
%endif

; =============================================================
