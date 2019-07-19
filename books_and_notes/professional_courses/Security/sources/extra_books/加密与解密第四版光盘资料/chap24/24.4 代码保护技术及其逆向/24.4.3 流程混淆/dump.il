
//  Microsoft (R) .NET Framework IL Disassembler.  Version 2.0.50727.42
//  Copyright (c) Microsoft Corporation.  All rights reserved.



// Metadata version: v2.0.50727
.assembly extern mscorlib
{
  .publickeytoken = (B7 7A 5C 56 19 34 E0 89 )                         // .z\V.4..
  .ver 2:0:0:0
}
.assembly sample1041
{
  .custom instance void [mscorlib]System.Runtime.CompilerServices.CompilationRelaxationsAttribute::.ctor(int32) = ( 01 00 08 00 00 00 00 00 ) 
  .custom instance void [mscorlib]System.Runtime.CompilerServices.RuntimeCompatibilityAttribute::.ctor() = ( 01 00 01 00 54 02 16 57 72 61 70 4E 6F 6E 45 78   // ....T..WrapNonEx
                                                                                                             63 65 70 74 69 6F 6E 54 68 72 6F 77 73 01 )       // ceptionThrows.
  .publickey = (00 24 00 00 04 80 00 00 94 00 00 00 06 02 00 00   // .$..............
                00 24 00 00 52 53 41 31 00 04 00 00 01 00 01 00   // .$..RSA1........
                3F 8C A8 ED C0 77 E9 53 17 EC B6 6D D9 35 03 84   // ?....w.S...m.5..
                DC BC D6 FF 3F 97 96 9B 82 79 68 22 49 D0 ED 82   // ....?....yh"I...
                52 53 DB A0 28 9A FE 8A C8 1A 60 1C B4 2F 70 D1   // RS..(.....`../p.
                32 FD AA 64 E4 E4 EF 22 4E 9C C7 AA 40 DD AD DC   // 2..d..."N...@...
                DD 2A CC 93 DE 9D 92 2C D9 DE EA 3B B9 0B 00 96   // .*.....,...;....
                93 0B 3E 0B 23 8F B5 A3 19 1F 26 4E E6 84 7B 13   // ..>.#.....&N..{.
                7A 6F 0E 63 A1 8E A3 93 C7 7F 2F 50 3C F2 EA B4   // zo.c....../P<...
                A2 09 DB 50 8F 53 EA CE 9C C8 A3 21 42 BD 2F D2 ) // ...P.S.....!B./.
  .hash algorithm 0x00008004
  .ver 0:0:0:0
}
.module sample1041.exe
// MVID: {6BD82C2B-DE04-4D5C-894F-C8CD01688597}
.imagebase 0x00400000
.file alignment 0x00000200
.stackreserve 0x00100000
.subsystem 0x0003       // WINDOWS_CUI
.corflags 0x00000009    //  ILONLY
// Image base: 0x03100000


// =============== CLASS MEMBERS DECLARATION ===================

.class private auto ansi beforefieldinit tankaiha.sample1041.class1
       extends [mscorlib]System.Object
{
  .method public hidebysig static void  Main() cil managed
  {
    .entrypoint
    // Code size       52 (0x34)
    .maxstack  2
    .locals init (int32 V_0,
             bool V_1)
    IL_0000:  nop
    br.s IL_0001

    
    IL_0002:  stloc.0
    IL_0003:  ldloc.0
    IL_0004:  ldc.i4.1
    IL_0005:  ceq
    IL_0007:  ldc.i4.0
    IL_0008:  ceq
    IL_000a:  stloc.1
    IL_000b:  ldloc.1
    IL_000c:  brtrue.s   IL_0028

    IL_000e:  nop
    IL_000f:  ldstr      "i="
    IL_0014:  ldloca.s   V_0
    IL_0016:  call       instance string [mscorlib]System.Int32::ToString()
    IL_001b:  call       string [mscorlib]System.String::Concat(string,
                                                                string)
    IL_0020:  call       void [mscorlib]System.Console::WriteLine(string)
    IL_0025:  nop
    IL_0026:  br.s       IL_0033

    IL_0028:  ldstr      "i modified"
    IL_002d:  call       void [mscorlib]System.Console::WriteLine(string)
    IL_0032:  nop
    IL_0033:  ret

    IL_0001:  ldc.i4.1
    br.s IL_0002
  } // end of method class1::Main

  .method public hidebysig specialname rtspecialname 
          instance void  .ctor() cil managed
  {
    // Code size       7 (0x7)
    .maxstack  8
    IL_0000:  ldarg.0
    IL_0001:  call       instance void [mscorlib]System.Object::.ctor()
    IL_0006:  ret
  } // end of method class1::.ctor

} // end of class tankaiha.sample1041.class1


// =============================================================

// *********** DISASSEMBLY COMPLETE ***********************
// WARNING: Created Win32 resource file F:\computer\MyWork\加密与解密3\附件\10.4\10.4.3\dump.res
