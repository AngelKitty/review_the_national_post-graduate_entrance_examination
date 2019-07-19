;;
;; aPLib compression library  -  the smaller the better :)
;;
;; NASM safe assembler wrapper for aP_depack_asm_safe
;;
;; Copyright (c) 1998-2004 by Joergen Ibsen / Jibz
;; All Rights Reserved
;;
;; http://www.ibsensoftware.com/
;;

bits 32

%include "nasmlcm.inc"

section lcmtext

lcmglobal aPsafe_depack,16

lcmexport aPsafe_depack,16

lcmextern aP_depack_asm_safe,16
lcmextern aP_crc32,8

; =============================================================

lcmlabel aPsafe_depack,16
    ; aPsafe_depack(const void *source,
    ;               size_t srclen,
    ;               void *destination
    ;               size_t dstlen);

    .ret$  equ 7*4
    .src$  equ 8*4 + 4
    .slen$ equ 8*4 + 8
    .dst$  equ 8*4 + 12
    .dlen$ equ 8*4 + 16

    pushad

    mov    esi, [esp + .src$]  ; esi -> inbuffer
    mov    ecx, [esp + .slen$] ; ecx =  srclen
    mov    edi, [esp + .dst$]  ; edi -> outbuffer

    test   esi, esi
    jz     short .return_error

    test   edi, edi
    jz     short .return_error

    cmp    ecx, byte 24        ; check srclen >= 24
    jb     short .return_error ;

    mov    ebx, [esi]         ; ebx = header.tag

    cmp    ebx, 032335041h    ; check tag == 'AP32'
    jne    short .return_error

    mov    ebx, [esi + 4]     ; ebx = header.header_size
    cmp    ebx, byte 24       ; check header_size >= 24
    jb     short .return_error

    sub    ecx, ebx            ; ecx = srclen without header
    jc     short .return_error ;

    cmp    [esi + 8], ecx      ; check header.packed_size is
    ja     short .return_error ; within remaining srclen

    add    ebx, esi           ; ebx -> packed data

    push   dword [esi + 8]    ; push header.packed_size
    push   ebx
    call   aP_crc32
    lcmadjust 8               ; adjust stack (if function is cdecl)

    cmp    eax, [esi + 12]    ; check eax == header.packed_crc
    jne    short .return_error

    mov    ecx, [esp + .dlen$] ; ecx = dstlen
    cmp    [esi + 16], ecx     ; check header.orig_size is ok
    ja     short .return_error

    push   ecx                ; push dstlen
    push   edi
    push   dword [esi + 8]    ; push header.packed_size
    push   ebx
    call   aP_depack_asm_safe
    lcmadjust 16              ; adjust stack (if function is cdecl)

    cmp    eax, [esi + 16]    ; check eax == header.orig_size
    jne    short .return_error

    mov    ebx, eax           ; ebx = unpacked size

    push   eax
    push   edi
    call   aP_crc32
    lcmadjust 8               ; adjust stack (if function is cdecl)

    cmp    eax, [esi + 20]    ; check eax == header.orig_crc

    mov    eax, ebx           ; eax = unpacked size

    je     short .return_eax

  .return_error:
    or     eax, byte -1       ; eax = -1

  .return_eax:
    mov    [esp + .ret$], eax ; return unpacked length in eax

    popad
    lcmret 16

; =============================================================

%ifdef LCM_OBJ
  section lcmdata
%endif

; =============================================================
