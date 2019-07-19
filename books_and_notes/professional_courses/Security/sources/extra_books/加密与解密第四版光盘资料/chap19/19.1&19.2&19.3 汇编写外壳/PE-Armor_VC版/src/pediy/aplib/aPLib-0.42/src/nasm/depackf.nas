;;
;; aPLib compression library  -  the smaller the better :)
;;
;; NASM fast assembler depacker
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

lcmglobal aP_depack_asm_fast,8

lcmexport aP_depack_asm_fast,8

; =============================================================

%macro getbitM 0
    add    dl, dl
    jnz    short %%stillbitsleft
    mov    dl, [esi]
    inc    esi
    adc    dl, dl
%%stillbitsleft:
%endmacro

%macro domatchM 1
    push   esi
    mov    esi, edi
    sub    esi, %1
    rep    movsb
    pop    esi
%endmacro

%macro getgammaM 1
    mov    %1, 1
%%getmore:
    getbitM
    adc    %1, %1
    getbitM
    jc     short %%getmore
%endmacro

; =============================================================

lcmlabel aP_depack_asm_fast,8
    ; aP_depack_asm_fast(const void *source, void *destination);

    _ret$  equ 7*4
    _src$  equ 8*4 + 4
    _dst$  equ 8*4 + 8

    pushad

    mov    esi, [esp + _src$] ; C calling convention
    mov    edi, [esp + _dst$]

    cld
    mov    dl, 80h

literal:
    mov    al, [esi]
    add    esi, byte 1
    mov    [edi], al
    add    edi, byte 1

    mov    ebx, 2

nexttag:
    getbitM
    jnc    short literal

    getbitM
    jnc    short codepair

    xor    eax, eax
    getbitM
    jnc    near shortmatch

    getbitM
    adc    eax, eax
    getbitM
    adc    eax, eax
    getbitM
    adc    eax, eax
    getbitM
    adc    eax, eax
    jz     short .thewrite
    push   edi
    sub    edi, eax
    mov    al, [edi]
    pop    edi
  .thewrite:
    mov    [edi], al
    inc    edi
    mov    ebx, 2
    jmp    short nexttag

codepair:
    getgammaM eax
    sub    eax, ebx
    mov    ebx, 1
    jnz    short normalcodepair
    getgammaM ecx
    domatchM ebp
    jmp    near nexttag

normalcodepair:
    dec    eax
    shl    eax, 8
    mov    al, [esi]
    inc    esi
    mov    ebp, eax
    getgammaM ecx

    cmp    eax, 32000
    sbb    ecx, byte -1

    cmp    eax, 1280
    sbb    ecx, byte -1

    cmp    eax, 128
    adc    ecx, byte 0

    cmp    eax, 128
    adc    ecx, byte 0

    domatchM eax
    jmp    near nexttag

shortmatch:
    mov    al, [esi]
    inc    esi
    xor    ecx, ecx
    db     0c0h, 0e8h, 001h
    jz     short donedepacking
    adc    ecx, BYTE 2
    mov    ebp, eax
    domatchM eax
    mov    ebx, 1
    jmp    near nexttag

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
