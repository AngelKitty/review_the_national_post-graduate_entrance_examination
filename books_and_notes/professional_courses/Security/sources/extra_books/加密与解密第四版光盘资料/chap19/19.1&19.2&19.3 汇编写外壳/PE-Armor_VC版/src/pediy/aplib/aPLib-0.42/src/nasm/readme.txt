- These versions can be compiled with NASM. To compile, use eg.

    COFF  : nasm.exe -f coff depack.nas
            nasm.exe -f coff depackf.nas

    ELF   : nasm.exe -f elf -D_ELF_ depack.nas
            nasm.exe -f elf -D_ELF_ depackf.nas

    OBJ   : nasm.exe -f obj -D_OBJ_ depack.nas
            nasm.exe -f obj -D_OBJ_ depackf.nas

    nasm.exe -f obj depack16.nas

- The COFF objects work with DJGPP, Watcom and VC linkers. The ELF objects
  work with GCC on Linux, FreeBSD and QNX. The OBJ objects work with (at
  least) Delphi, Watcom. The 16bit version works with (at least) TASM and BC.

- In Watcom and VC files, you can use the following function declaration
  (modify if you are using aP_depack_asm_fast):

    #ifdef __cplusplus
    extern "C" {
    #endif

    unsigned int __cdecl aP_depack_asm(const void *, void *);

    #ifdef __cplusplus
    } /* extern "C" */
    #endif

- In DJGPP and GCC files, you can use the following declaration
  (modify if you are using aP_depack_asm_fast):

    #ifdef __cplusplus
    extern "C" {
    #endif

    unsigned int aP_depack_asm(const void *, void *);

    #ifdef __cplusplus
    } /* extern "C" */
    #endif

- In BC (DOS) files, you can use the following function declaration to use
  the 16bit depacker:

    #ifdef __cplusplus
    extern "C" {
    #endif

    unsigned long aP_depack16_asm(const void far *, void far *);

    #ifdef __cplusplus
    } /* extern "C" */
    #endif
