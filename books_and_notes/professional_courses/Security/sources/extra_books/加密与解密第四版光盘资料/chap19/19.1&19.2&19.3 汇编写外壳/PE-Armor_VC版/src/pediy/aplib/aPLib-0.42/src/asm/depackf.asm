;;
;; aPLib compression library  -  the smaller the better :)
;;
;; TASM / MASM / WASM fast assembler depacker
;;
;; Copyright (c) 1998-2004 by Joergen Ibsen / Jibz
;; All Rights Reserved
;;
;; http://www.ibsensoftware.com/
;;

.386p
.MODEL flat

getbitM MACRO
LOCAL stillbitsleft
    add    dl, dl
    jnz    stillbitsleft
    mov    dl, [esi]
    inc    esi
    adc    dl, dl
stillbitsleft:
ENDM getbitM

domatchM MACRO reg
    push   esi
    mov    esi, edi
    sub    esi, reg
    rep    movsb
    pop    esi
ENDM domatchM

getgammaM MACRO reg
LOCAL getmorebits
    mov    reg, 1
getmorebits:
    getbitM
    adc    reg, reg
    getbitM
    jc     getmorebits
ENDM getgammaM

.CODE

PUBLIC _aP_depack_asm_fast

_aP_depack_asm_fast:
    pushad

    mov    esi, [esp + 36]    ; C calling convention
    mov    edi, [esp + 40]

    cld
    mov    dl, 80h

literal:
    mov    al, [esi]
    inc    esi
    mov    [edi], al
    inc    edi

    mov    ebx, 2

nexttag:
    getbitM
    jnc    literal

    getbitM
    jnc    codepair

    xor    eax, eax
    getbitM
    jnc    shortmatch

    getbitM
    adc    eax, eax
    getbitM
    adc    eax, eax
    getbitM
    adc    eax, eax
    getbitM
    adc    eax, eax
    jz     thewrite
    push   edi
    sub    edi, eax
    mov    al, [edi]
    pop    edi
thewrite:
    mov    [edi], al
    inc    edi
    mov    ebx, 2
    jmp    short nexttag

codepair:
    getgammaM eax
    sub    eax, ebx
    mov    ebx, 1
    jnz    normalcodepair
    getgammaM ecx
    domatchM ebp
    jmp    nexttag

normalcodepair:
    dec    eax
    shl    eax, 8
    mov    al, [esi]
    inc    esi
    mov    ebp, eax
    getgammaM ecx

    cmp    eax, 32000
    sbb    ecx, -1

    cmp    eax, 1280
    sbb    ecx, -1

    cmp    eax, 128
    adc    ecx, 0

    cmp    eax, 128
    adc    ecx, 0

    domatchM eax
    jmp    nexttag

shortmatch:
    mov    al, [esi]
    inc    esi
    xor    ecx, ecx
    db     0c0h, 0e8h, 001h
    jz     donedepacking
    adc    ecx, 2
    mov    ebp, eax
    domatchM eax
    mov    ebx, 1
    jmp    nexttag

donedepacking:
    sub    edi, [esp + 40]
    mov    [esp + 28], edi    ; return unpacked length in eax

    popad
    ret

END
