;;
;; NASM linker compatibility macros 2004.06.13
;;
;; Copyright (c) 2001-2004 by Joergen Ibsen / Jibz
;; All Rights Reserved
;;
;; http://www.ibsensoftware.com/
;;

; define LCM_ELF for ELF32 object files
; define LCM_OBJ for OMF object files
; define LCM_OBJ and LCM_DLL for OMF object files for a dll (stdcall)
; define LCM_MSLIBS/LCM_MSIMP for MS style import libs (lcmwinextern)
; default is DJGPP/WIN32 COFF object files

; remember to do lcm*extern before lcmimport

; ====================================================================
;
; There are differences between how the object formats that NASM
; supports work, and what features they support. Similarly there
; are differences between how complete and standard compliant the
; support for these formats are in linkers.
;
; The NASM linker compatibility macros (nasmlcm) were put together
; to ease my work by allowing a single source file to be assembled
; for use with a number of compilers/linkers.
;
; Currently obj/omf, win32/coff, djgpp/coff and elf32 output formats
; are supported. The following macros are available:
;
;   lcmtext       - section name for the code section
;   lcmdata       - section name for the initialized data section
;   lcmbss        - section name for the uninitialized data section
;
;   lcmglobal     - declare a function (two arguments) or data (one
;                   argument) as global in the current format
;   lcmcglobal    - same as lcmglobal, but uses C name decoration
;
;   lcmextern     - declare a function (two arguments) or data (one
;                   argument) as extern in the current format
;   lcmcextern    - same as lcmextern, but uses C name decoration
;   lcmdllextern  - same as lcmextern, but uses dll name decoration
;   lcmwinextern  - same as lcmextern, but uses name decoration for
;                   calling Win32 Api functions (see LCM_MSLIBS)
;
;   lcmimport     - declares a function (two arguments) or data (one
;                   argument) as imported in the current format
;   lcmexport     - declares a function (two arguments) or data (one
;                   argument) as exported in the current format
;
;   lcmlabel      - start label for a function in the current format
;   lcmclabel     - start label for a function with C name decoration
;   lcmadjust     - adjust stack after a function call in the current
;                   format
;   lcmret        - return from a function in the current format
;   lcmcret       - return from a C function
;
; The following defines change the format and behaviour:
;
;   LCM_ELF     -  the lcm*global macro adds :function and :data type
;                  specifiers
;
;   LCM_OBJ     -  section names are similar to those produced by
;                  Borland tools to increase compatibility with
;                  various OMF compatible linkers
;
;   LCM_DLL     -  functions are exported and imported with added
;                  size specifiers (_SomeFunction@12), lcmret adjusts
;                  stack (stdcall)
;
;   LCM_MSLIBS  -  the lcmwinextern macro prepends an underscore and
;                  adds size specification for functions, allowing
;                  the object file to be linked with MS libraries.
;                  This produces indirect calls through a jump table.
;
;   LCM_MSIMP   -  the lcmwinextern macro prepends '__imp__' and adds
;                  size specification for functions, allowing the
;                  object file to be linked with MS libraries. This
;                  produces direct calls through the import table.
;
; ====================================================================

%ifndef NASMLCM_INC_INCLUDED
%define NASMLCM_INC_INCLUDED

%ifdef LCM_DLL
  %ifndef LCM_OBJ
    %error "LCM_DLL needs LCM_OBJ defined!"
  %endif
%endif

; --- define lcm- section names ---
;
;  a number of linkers require omf objects where the section
;  names are equal to those produces by tasm.

%ifdef LCM_OBJ

  %define lcmtext _TEXT class=CODE public use32 align=4 FLAT
  %define lcmdata _DATA class=DATA public use32 align=4
  %define lcmbss _BSS class=BSS public use32 align=4 FLAT

  group FLAT
  group DGROUP _DATA

%else ; LCM_OBJ

  %define lcmtext .text
  %define lcmdata .data
  %define lcmbss .bss

%endif ; LCM_OBJ

; --- define lcmglobal and lcm*extern macros ---
;
;  special handling of functions and data for ELF32

%ifdef LCM_ELF

  %macro lcmglobal 2
    global %{1}:function
  %endmacro
  %macro lcmglobal 1
    global %{1}:data
  %endmacro

  %define lcmcglobal lcmglobal

  %macro lcmextern 1-2
    extern %1
  %endmacro

  %macro lcmcextern 0
    %error lcmcextern not supported in ELF format
  %endmacro

  %macro lcmdllextern 0
    %error lcmdllextern not supported in ELF format
  %endmacro

%else ; LCM_ELF

  %ifdef LCM_DLL

    %macro lcmglobal 2
      global _%1
      global _%1@%2
    %endmacro
    %macro lcmglobal 1
      global _%1
      %define %1 _%1
    %endmacro

    %macro lcmcglobal 2
      global _%1
    %endmacro
    %macro lcmcglobal 1
      global _%1
      %define %1 _%1
    %endmacro

    %macro lcmextern 2
      extern _%1@%2
      %define %1 _%1@%2
    %endmacro
    %macro lcmextern 1
      extern _%1
      %define %1 _%1
    %endmacro

  %else

    %macro lcmglobal 2
      global _%1
    %endmacro
    %macro lcmglobal 1
      global _%1
      %define %1 _%1
    %endmacro

    %define lcmcglobal lcmglobal

    %macro lcmextern 1-2
      extern _%1
      %define %1 _%1
    %endmacro

  %endif

  %macro lcmcextern 1-2
    extern _%1
    %define %1 _%1
  %endmacro

  %macro lcmdllextern 2
    extern _%1@%2
    %define %1 _%1@%2
  %endmacro
  %macro lcmdllextern 1
    extern _%1
    %define %1 _%1
  %endmacro

  %macro lcmwinextern 2
    %ifdef LCM_MSLIBS
      extern _%1@%2
      %define %1 _%1@%2
    %else
      %ifdef LCM_MSIMP
        extern __imp__%1@%2
        %define %1 [__imp__%1@%2]
      %else
        extern %1
      %endif
    %endif
  %endmacro

%endif ; LCM_ELF

; --- define lcmimport and lcmexport ---
;

%ifdef LCM_OBJ

  %macro lcmimport 2-3
    import %1 %2 %3
    %rotate 1
  %endmacro

  %ifdef LCM_DLL

    %macro lcmexport 2
      export _%1
      export _%1@%2
    %endmacro
    %macro lcmexport 1
      export _%1
    %endmacro

  %else

    %macro lcmexport 1-2
    %endmacro

  %endif

%else ; LCM_OBJ

  %macro lcmimport 2-3
  %endmacro

  %macro lcmexport 1-2
  %endmacro

%endif ; LCM_OBJ

; --- define lcmlabel, lcmadjust and lcmret macros ---
;
;  we need special labels and stdcall calling convention when
;  assembling for a dll

%ifdef LCM_ELF

  %macro lcmlabel 2
    %1:
  %endmacro

  %define lcmclabel lcmlabel

  %macro lcmadjust 1
    %if %1 < 128
      %if %1 > 0
        add esp, byte %1
      %endif
    %else
      add esp, %1
    %endif
  %endmacro

  %macro lcmret 1
    ret
  %endmacro

  %define lcmcret lcmret

%else ; LCM_ELF

  %ifdef LCM_DLL
    %macro lcmlabel 2
      _%1:
      _%1@%2:
    %endmacro

    %macro lcmclabel 2
      _%1:
    %endmacro

    %macro lcmadjust 1
    %endmacro

    %macro lcmret 1
      %if %1 > 0
        ret %1
      %else
        ret
      %endif
    %endmacro

    %macro lcmcret 1
      ret
    %endmacro

  %else

    %macro lcmlabel 2
      _%1:
    %endmacro

    %define lcmclabel lcmlabel

    %macro lcmadjust 1
      %if %1 < 128
        %if %1 > 0
          add esp, byte %1
        %endif
      %else
        add esp, %1
      %endif
    %endmacro

    %macro lcmret 1
      ret
    %endmacro

    %define lcmcret lcmret
  %endif

%endif ; LCM_ELF

%endif ; NASMLCM_INC_INCLUDED
