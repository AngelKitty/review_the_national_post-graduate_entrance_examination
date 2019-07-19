;;
;; aPLib compression library  -  the smaller the better :)
;;
;; NASM safe assembler crc checker
;;
;; Copyright (c) 1998-2004 by Joergen Ibsen / Jibz
;; All Rights Reserved
;;
;; http://www.ibsensoftware.com/
;;

bits 32

%include "nasmlcm.inc"

section lcmtext

lcmglobal aPsafe_check,4

lcmexport aPsafe_check,4

lcmextern aP_crc32,8

; =============================================================

lcmlabel aPsafe_check,4
    ; aPsafe_check(const void *source);

    .ret$  equ 7*4
    .src$  equ 8*4 + 4

    pushad

    mov    esi, [esp + .src$] ; esi -> buffer

    mov    ebx, [esi]         ; ebx = header.tag

    cmp    ebx, 032335041h    ; check tag == 'AP32'
    jne    short .return_error

    mov    ebx, [esi + 4]     ; ebx = header.header_size
    cmp    ebx, byte 24       ; check header_size >= 24
    jb     short .return_error

    add    ebx, esi           ; ebx -> packed data

    push   dword [esi + 8]    ; push header.packed_size
    push   ebx
    call   aP_crc32
    lcmadjust 8               ; adjust stack (if function is cdecl)

    cmp    eax, [esi + 12]    ; check eax == header.packed_crc

    mov    eax, [esi + 16]    ; eax = header.orig_size

    je     short .return_eax

  .return_error:
    or     eax, byte -1       ; eax = -1

  .return_eax:
    mov    [esp + .ret$], eax ; return unpacked length in eax

    popad
    lcmret 4

; =============================================================

%ifdef LCM_OBJ
  section lcmdata
%endif

; =============================================================
