- These versions can be compiled with TASM, MASM or WASM. To compile,
  use eg. (TASM)

    tasm32.exe /mx /m /q /zn depack.asm
    tasm32.exe /mx /m /q /zn depackf.asm
    tasm.exe /mx /m /q /zn depack16.asm

  or (MASM)

    ml.exe /Cp /c depack.asm
    ml.exe /Cp /c depackf.asm
    ml.exe /Cp /c depack16.asm

  or (WASM)

    wasm.exe depack.asm
    wasm.exe depackf.asm
    wasm.exe depack16.asm

- They work with Delphi, Watcom, VC and TASM32 (should work with most other
  32-bit linkers too). The 16bit version works with (at least) TASM and BC.

- In Watcom and VC files, you can use the following function declaration
  (modify if you are using aP_depack_asm_fast):

    #ifdef __cplusplus
    extern "C" {
    #endif

    unsigned int __cdecl aP_depack_asm(const void *, void *);

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
