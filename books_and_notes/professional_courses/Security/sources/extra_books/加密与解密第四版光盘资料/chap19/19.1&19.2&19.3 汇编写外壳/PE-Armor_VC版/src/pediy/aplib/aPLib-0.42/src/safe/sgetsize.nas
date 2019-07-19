;;
;; aPLib compression library  -  the smaller the better :)
;;
;; NASM safe assembler header access
;;
;; Copyright (c) 1998-2004 by Joergen Ibsen / Jibz
;; All Rights Reserved
;;
;; http://www.ibsensoftware.com/
;;

bits 32

%include "nasmlcm.inc"

section lcmtext

lcmglobal aPsafe_get_orig_size,4

lcmexport aPsafe_get_orig_size,4

; =============================================================

lcmlabel aPsafe_get_orig_size,4
    ; aPsafe_get_orig_size(const void *source);

    .ret$  equ 7*4
    .src$  equ 8*4 + 4

    pushad

    mov    esi, [esp + .src$] ; esi -> buffer

    mov    ebx, [esi]         ; ebx = header.tag

    or     eax, byte -1       ; eax = -1

    cmp    ebx, 032335041h    ; check tag == 'AP32'
    jne    short .return_eax

    mov    ebx, [esi + 4]     ; ebx = header.header_size
    cmp    ebx, byte 24       ; check header_size >= 24
    jb     short .return_eax

    mov    eax, [esi + 16]    ; eax = header.orig_size

  .return_eax:
    mov    [esp + .ret$], eax ; return unpacked length in eax

    popad
    lcmret 4

; =============================================================

%ifdef LCM_OBJ
  section lcmdata
%endif

; =============================================================
