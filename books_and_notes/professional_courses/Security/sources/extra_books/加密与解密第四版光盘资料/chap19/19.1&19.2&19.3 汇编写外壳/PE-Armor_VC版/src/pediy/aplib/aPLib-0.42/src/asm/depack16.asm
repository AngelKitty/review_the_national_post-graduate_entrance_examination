;;
;; aPLib compression library  -  the smaller the better :)
;;
;; TASM / MASM / WASM 16bit assembler depacker
;;
;; Copyright (c) 1998-2004 by Joergen Ibsen / Jibz
;; All Rights Reserved
;;
;; http://www.ibsensoftware.com/
;;
;; -> 16bit by Metalbrain (metalbrain_coder@gmx.net)
;;

.MODEL TINY                   ; or whatever your code is :)

.CODE
.386

PUBLIC _aP_depack16_asm

_aP_depack16_asm:
    push   es
    push   ebp
    mov    bp, sp
    pushad

    mov    esi, [bp + 8]      ; C calling convention
    mov    edi, [bp + 12]

    push   bp

    xor    eax, eax           ; convert segm:offs to linear
    xchg   ax, si
    shr    esi, 12
    add    esi, eax

    xor    ax, ax             ; convert segm:offs to linear
    xchg   ax, di
    shr    edi, 12
    add    edi, eax

    push   edi

    cld
    mov    dl, 80h
literal:
    call   getesi
putedi_nexttag:
    call   putedi
    mov    dh, 1
    jmp    short nexttag

normalcodepair:
    xchg   ax, cx
    dec    ax
    shl    eax, 8
    call   getesi
    call   getgamma
    cmp    eax, 32000
    jae    short domatch_with_2inc
    cmp    ah, 5
    jae    short domatch_with_inc
    cmp    ax, 127
    ja     short domatch_new_lastpos

domatch_with_2inc:
    inc    ecx
domatch_with_inc:
    inc    ecx

domatch_new_lastpos:
    xchg   eax, ebp
domatch_lastpos:
    mov    eax, ebp
    mov    dh, 0

domatch:
    push   esi
    mov    esi, edi
    sub    esi, eax
repmovsb:
    call   getesi
    call   putedi
    loopd  repmovsb
    pop    esi

nexttag:
    call   getbit
    jnc    short literal
    xor    ecx, ecx
    call   getbit
    jnc    short codepair
    xor    eax, eax
    call   getbit
    jnc    short shortmatch
    mov    dh, 1
    inc    cx
    mov    al, 10h
getmorebits:
    call   getbit
    adc    al, al
    jnc    short getmorebits
    jnz    short domatch
    jmp    short putedi_nexttag

codepair:
    call   getgamma_no_ecx
    shr    dh, 1
    sbb    cx, 1
    jnz    short normalcodepair
    push   offset domatch_lastpos

getgamma:
    xor    ecx, ecx
getgamma_no_ecx:
    inc    cx
getgammaloop:
    call   getbit
    adc    ecx, ecx
    call   getbit
    jc     short getgammaloop
    ret

shortmatch:
    call   getesi
    shr    ax, 1
    jz     short donedepacking
    adc    cx, cx
    jmp    short domatch_with_2inc

getbit:
    add    dl, dl
    jnz    short stillbitsleft
    pushf
    xchg   ax, dx
    call   getesi
    xchg   ax, dx
    popf
    adc    dl, dl
stillbitsleft:
    ret

getesi:
    push   esi
    pop    bx
    pop    bx
    ror    bx, 4
    mov    es, bx
    mov    al, es:[si]
    inc    esi
    ret

putedi:
    push   edi
    pop    bx
    pop    bx
    ror    bx, 4
    mov    es, bx
    mov    es:[di], al
    inc    edi
    ret

donedepacking:
    pop    eax

    pop    bp
    sub    edi, eax
    mov    [bp - 4], edi      ; return unpacked length in eax

    popad
    pop    ebp
    pop    es
    ret

END
