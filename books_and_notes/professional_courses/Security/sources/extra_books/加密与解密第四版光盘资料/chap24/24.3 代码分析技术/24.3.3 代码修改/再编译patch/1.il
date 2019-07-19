
//  Microsoft (R) .NET Framework IL Disassembler.  Version 2.0.50727.42
//  Copyright (c) Microsoft Corporation.  All rights reserved.



// Metadata version: v2.0.50727
.assembly extern mscorlib
{
  .publickeytoken = (B7 7A 5C 56 19 34 E0 89 )                         // .z\V.4..
  .ver 2:0:0:0
}
.assembly extern System
{
  .publickeytoken = (B7 7A 5C 56 19 34 E0 89 )                         // .z\V.4..
  .ver 2:0:0:0
}
.assembly extern System.Windows.Forms
{
  .publickeytoken = (B7 7A 5C 56 19 34 E0 89 )                         // .z\V.4..
  .ver 2:0:0:0
}
.assembly extern System.Drawing
{
  .publickeytoken = (B0 3F 5F 7F 11 D5 0A 3A )                         // .?_....:
  .ver 2:0:0:0
}
.assembly WindowsApplication1
{
  .custom instance void [mscorlib]System.Reflection.AssemblyTrademarkAttribute::.ctor(string) = ( 01 00 00 00 00 ) 
  .custom instance void [mscorlib]System.Runtime.CompilerServices.RuntimeCompatibilityAttribute::.ctor() = ( 01 00 01 00 54 02 16 57 72 61 70 4E 6F 6E 45 78   // ....T..WrapNonEx
                                                                                                             63 65 70 74 69 6F 6E 54 68 72 6F 77 73 01 )       // ceptionThrows.
  .custom instance void [mscorlib]System.Reflection.AssemblyCompanyAttribute::.ctor(string) = ( 01 00 05 4E 45 33 36 35 00 00 )                   // ...NE365..
  .custom instance void [mscorlib]System.Reflection.AssemblyTitleAttribute::.ctor(string) = ( 01 00 16 4E 45 33 36 35 20 2E 4E 65 74 20 43 72   // ...NE365 .Net Cr
                                                                                              61 63 6B 4D 65 20 32 2E 31 00 00 )                // ackMe 2.1..
  .custom instance void [mscorlib]System.Runtime.CompilerServices.CompilationRelaxationsAttribute::.ctor(int32) = ( 01 00 08 00 00 00 00 00 ) 
  .custom instance void [mscorlib]System.Runtime.InteropServices.GuidAttribute::.ctor(string) = ( 01 00 24 61 39 65 38 61 63 35 36 2D 35 35 63 36   // ..$a9e8ac56-55c6
                                                                                                  2D 34 39 38 35 2D 39 32 31 31 2D 65 65 36 64 63   // -4985-9211-ee6dc
                                                                                                  33 31 61 32 65 64 30 00 00 )                      // 31a2ed0..
  .custom instance void [mscorlib]System.Reflection.AssemblyDescriptionAttribute::.ctor(string) = ( 01 00 16 4E 45 33 36 35 20 2E 4E 65 74 20 43 72   // ...NE365 .Net Cr
                                                                                                    61 63 6B 4D 65 20 32 2E 31 00 00 )                // ackMe 2.1..
  .custom instance void [mscorlib]System.Runtime.InteropServices.ComVisibleAttribute::.ctor(bool) = ( 01 00 00 00 00 ) 
  .custom instance void [mscorlib]System.Reflection.AssemblyCopyrightAttribute::.ctor(string) = ( 01 00 17 43 6F 70 79 72 69 67 68 74 20 C2 A9 20   // ...Copyright .. 
                                                                                                  4E 45 33 36 35 20 32 30 30 36 00 00 )             // NE365 2006..
  .custom instance void [mscorlib]System.Reflection.AssemblyProductAttribute::.ctor(string) = ( 01 00 0A 43 72 61 63 6B 4D 65 32 2E 31 00 00 )    // ...CrackMe2.1..
  .custom instance void [mscorlib]System.Reflection.AssemblyFileVersionAttribute::.ctor(string) = ( 01 00 07 32 2E 31 2E 30 2E 30 00 00 )             // ...2.1.0.0..
  .custom instance void [mscorlib]System.Reflection.AssemblyConfigurationAttribute::.ctor(string) = ( 01 00 00 00 00 ) 
  .hash algorithm 0x00008004
  .ver 2:1:0:0
}
.mresource public WindowsApplication1.Properties.Resources.resources
{
  // Offset: 0x00000000 Length: 0x00001059
  // WARNING: managed resource file WindowsApplication1.Properties.Resources.resources created
}
.mresource public WindowsApplication1.Form1.resources
{
  // Offset: 0x0000105D Length: 0x000000B4
  // WARNING: managed resource file WindowsApplication1.Form1.resources created
}
.module WindowsApplication1.exe
// MVID: {52844DA0-BED4-46FF-9FB5-F4C1FE2CB7A3}
.imagebase 0x00400000
.file alignment 0x00001000
.stackreserve 0x00100000
.subsystem 0x0002       // WINDOWS_GUI
.corflags 0x00000001    //  ILONLY
// Image base: 0x03140000


// =============== CLASS MEMBERS DECLARATION ===================

.class private abstract auto ansi sealed beforefieldinit WindowsApplication1.Program
       extends [mscorlib]System.Object
{
  .method private hidebysig static void  Main() cil managed
  {
    .entrypoint
    .custom instance void [mscorlib]System.STAThreadAttribute::.ctor() = ( 01 00 00 00 ) 
    // Code size       22 (0x16)
    .maxstack  8
    IL_0000:  call       void [System.Windows.Forms]System.Windows.Forms.Application::EnableVisualStyles()
    IL_0005:  ldc.i4.0
    IL_0006:  call       void [System.Windows.Forms]System.Windows.Forms.Application::SetCompatibleTextRenderingDefault(bool)
    IL_000b:  newobj     instance void WindowsApplication1.Form1::.ctor()
    IL_0010:  call       void [System.Windows.Forms]System.Windows.Forms.Application::Run(class [System.Windows.Forms]System.Windows.Forms.Form)
    IL_0015:  ret
  } // end of method Program::Main

} // end of class WindowsApplication1.Program

.class public auto ansi beforefieldinit WindowsApplication1.Form1
       extends [System.Windows.Forms]System.Windows.Forms.Form
{
  .field private class [System]System.ComponentModel.IContainer components
  .field private class [System.Windows.Forms]System.Windows.Forms.TextBox textBoxUser
  .field private class [System.Windows.Forms]System.Windows.Forms.TextBox textBoxKey
  .field private class [System.Windows.Forms]System.Windows.Forms.Label label1
  .field private class [System.Windows.Forms]System.Windows.Forms.Label label2
  .field private class [System.Windows.Forms]System.Windows.Forms.Button buttonTry
  .field private class [System.Windows.Forms]System.Windows.Forms.Button buttonAbout
  .field private class [System.Windows.Forms]System.Windows.Forms.PictureBox pictureBox1
  .field private uint8[] strName
  .method family hidebysig virtual instance void 
          Dispose(bool disposing) cil managed
  {
    // Code size       30 (0x1e)
    .maxstack  8
    IL_0000:  ldarg.1
    IL_0001:  brfalse.s  IL_0016

    IL_0003:  ldarg.0
    IL_0004:  ldfld      class [System]System.ComponentModel.IContainer WindowsApplication1.Form1::components
    IL_0009:  brfalse.s  IL_0016

    IL_000b:  ldarg.0
    IL_000c:  ldfld      class [System]System.ComponentModel.IContainer WindowsApplication1.Form1::components
    IL_0011:  callvirt   instance void [mscorlib]System.IDisposable::Dispose()
    IL_0016:  ldarg.0
    IL_0017:  ldarg.1
    IL_0018:  call       instance void [System.Windows.Forms]System.Windows.Forms.Form::Dispose(bool)
    IL_001d:  ret
  } // end of method Form1::Dispose

  .method private hidebysig instance void 
          InitializeComponent() cil managed
  {
    // Code size       1027 (0x403)
    .maxstack  4
    IL_0000:  ldarg.0
    IL_0001:  newobj     instance void [System.Windows.Forms]System.Windows.Forms.TextBox::.ctor()
    IL_0006:  stfld      class [System.Windows.Forms]System.Windows.Forms.TextBox WindowsApplication1.Form1::textBoxUser
    IL_000b:  ldarg.0
    IL_000c:  newobj     instance void [System.Windows.Forms]System.Windows.Forms.TextBox::.ctor()
    IL_0011:  stfld      class [System.Windows.Forms]System.Windows.Forms.TextBox WindowsApplication1.Form1::textBoxKey
    IL_0016:  ldarg.0
    IL_0017:  newobj     instance void [System.Windows.Forms]System.Windows.Forms.Label::.ctor()
    IL_001c:  stfld      class [System.Windows.Forms]System.Windows.Forms.Label WindowsApplication1.Form1::label1
    IL_0021:  ldarg.0
    IL_0022:  newobj     instance void [System.Windows.Forms]System.Windows.Forms.Label::.ctor()
    IL_0027:  stfld      class [System.Windows.Forms]System.Windows.Forms.Label WindowsApplication1.Form1::label2
    IL_002c:  ldarg.0
    IL_002d:  newobj     instance void [System.Windows.Forms]System.Windows.Forms.Button::.ctor()
    IL_0032:  stfld      class [System.Windows.Forms]System.Windows.Forms.Button WindowsApplication1.Form1::buttonTry
    IL_0037:  ldarg.0
    IL_0038:  newobj     instance void [System.Windows.Forms]System.Windows.Forms.Button::.ctor()
    IL_003d:  stfld      class [System.Windows.Forms]System.Windows.Forms.Button WindowsApplication1.Form1::buttonAbout
    IL_0042:  ldarg.0
    IL_0043:  newobj     instance void [System.Windows.Forms]System.Windows.Forms.PictureBox::.ctor()
    IL_0048:  stfld      class [System.Windows.Forms]System.Windows.Forms.PictureBox WindowsApplication1.Form1::pictureBox1
    IL_004d:  ldarg.0
    IL_004e:  ldfld      class [System.Windows.Forms]System.Windows.Forms.PictureBox WindowsApplication1.Form1::pictureBox1
    IL_0053:  callvirt   instance void [System]System.ComponentModel.ISupportInitialize::BeginInit()
    IL_0058:  ldarg.0
    IL_0059:  call       instance void [System.Windows.Forms]System.Windows.Forms.Control::SuspendLayout()
    IL_005e:  ldarg.0
    IL_005f:  ldfld      class [System.Windows.Forms]System.Windows.Forms.TextBox WindowsApplication1.Form1::textBoxUser
    IL_0064:  ldc.i4.s   52
    IL_0066:  ldc.i4.s   56
    IL_0068:  newobj     instance void [System.Drawing]System.Drawing.Point::.ctor(int32,
                                                                                   int32)
    IL_006d:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Location(valuetype [System.Drawing]System.Drawing.Point)
    IL_0072:  ldarg.0
    IL_0073:  ldfld      class [System.Windows.Forms]System.Windows.Forms.TextBox WindowsApplication1.Form1::textBoxUser
    IL_0078:  ldstr      "textBoxUser"
    IL_007d:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Name(string)
    IL_0082:  ldarg.0
    IL_0083:  ldfld      class [System.Windows.Forms]System.Windows.Forms.TextBox WindowsApplication1.Form1::textBoxUser
    IL_0088:  ldc.i4.s   126
    IL_008a:  ldc.i4.s   21
    IL_008c:  newobj     instance void [System.Drawing]System.Drawing.Size::.ctor(int32,
                                                                                  int32)
    IL_0091:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Size(valuetype [System.Drawing]System.Drawing.Size)
    IL_0096:  ldarg.0
    IL_0097:  ldfld      class [System.Windows.Forms]System.Windows.Forms.TextBox WindowsApplication1.Form1::textBoxUser
    IL_009c:  ldc.i4.0
    IL_009d:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_TabIndex(int32)
    IL_00a2:  ldarg.0
    IL_00a3:  ldfld      class [System.Windows.Forms]System.Windows.Forms.TextBox WindowsApplication1.Form1::textBoxKey
    IL_00a8:  ldc.i4.s   52
    IL_00aa:  ldc.i4.s   94
    IL_00ac:  newobj     instance void [System.Drawing]System.Drawing.Point::.ctor(int32,
                                                                                   int32)
    IL_00b1:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Location(valuetype [System.Drawing]System.Drawing.Point)
    IL_00b6:  ldarg.0
    IL_00b7:  ldfld      class [System.Windows.Forms]System.Windows.Forms.TextBox WindowsApplication1.Form1::textBoxKey
    IL_00bc:  ldstr      "textBoxKey"
    IL_00c1:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Name(string)
    IL_00c6:  ldarg.0
    IL_00c7:  ldfld      class [System.Windows.Forms]System.Windows.Forms.TextBox WindowsApplication1.Form1::textBoxKey
    IL_00cc:  ldc.i4.s   126
    IL_00ce:  ldc.i4.s   21
    IL_00d0:  newobj     instance void [System.Drawing]System.Drawing.Size::.ctor(int32,
                                                                                  int32)
    IL_00d5:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Size(valuetype [System.Drawing]System.Drawing.Size)
    IL_00da:  ldarg.0
    IL_00db:  ldfld      class [System.Windows.Forms]System.Windows.Forms.TextBox WindowsApplication1.Form1::textBoxKey
    IL_00e0:  ldc.i4.1
    IL_00e1:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_TabIndex(int32)
    IL_00e6:  ldarg.0
    IL_00e7:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Label WindowsApplication1.Form1::label1
    IL_00ec:  ldc.i4.1
    IL_00ed:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_AutoSize(bool)
    IL_00f2:  ldarg.0
    IL_00f3:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Label WindowsApplication1.Form1::label1
    IL_00f8:  ldc.i4.5
    IL_00f9:  ldc.i4.s   59
    IL_00fb:  newobj     instance void [System.Drawing]System.Drawing.Point::.ctor(int32,
                                                                                   int32)
    IL_0100:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Location(valuetype [System.Drawing]System.Drawing.Point)
    IL_0105:  ldarg.0
    IL_0106:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Label WindowsApplication1.Form1::label1
    IL_010b:  ldstr      "label1"
    IL_0110:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Name(string)
    IL_0115:  ldarg.0
    IL_0116:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Label WindowsApplication1.Form1::label1
    IL_011b:  ldc.i4.s   29
    IL_011d:  ldc.i4.s   12
    IL_011f:  newobj     instance void [System.Drawing]System.Drawing.Size::.ctor(int32,
                                                                                  int32)
    IL_0124:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Size(valuetype [System.Drawing]System.Drawing.Size)
    IL_0129:  ldarg.0
    IL_012a:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Label WindowsApplication1.Form1::label1
    IL_012f:  ldc.i4.2
    IL_0130:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_TabIndex(int32)
    IL_0135:  ldarg.0
    IL_0136:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Label WindowsApplication1.Form1::label1
    IL_013b:  ldstr      "User"
    IL_0140:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Text(string)
    IL_0145:  ldarg.0
    IL_0146:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Label WindowsApplication1.Form1::label2
    IL_014b:  ldc.i4.1
    IL_014c:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_AutoSize(bool)
    IL_0151:  ldarg.0
    IL_0152:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Label WindowsApplication1.Form1::label2
    IL_0157:  ldc.i4.5
    IL_0158:  ldc.i4.s   97
    IL_015a:  newobj     instance void [System.Drawing]System.Drawing.Point::.ctor(int32,
                                                                                   int32)
    IL_015f:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Location(valuetype [System.Drawing]System.Drawing.Point)
    IL_0164:  ldarg.0
    IL_0165:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Label WindowsApplication1.Form1::label2
    IL_016a:  ldstr      "label2"
    IL_016f:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Name(string)
    IL_0174:  ldarg.0
    IL_0175:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Label WindowsApplication1.Form1::label2
    IL_017a:  ldc.i4.s   23
    IL_017c:  ldc.i4.s   12
    IL_017e:  newobj     instance void [System.Drawing]System.Drawing.Size::.ctor(int32,
                                                                                  int32)
    IL_0183:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Size(valuetype [System.Drawing]System.Drawing.Size)
    IL_0188:  ldarg.0
    IL_0189:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Label WindowsApplication1.Form1::label2
    IL_018e:  ldc.i4.3
    IL_018f:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_TabIndex(int32)
    IL_0194:  ldarg.0
    IL_0195:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Label WindowsApplication1.Form1::label2
    IL_019a:  ldstr      "Key"
    IL_019f:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Text(string)
    IL_01a4:  ldarg.0
    IL_01a5:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Button WindowsApplication1.Form1::buttonTry
    IL_01aa:  ldc.i4     0xb8
    IL_01af:  ldc.i4.s   56
    IL_01b1:  newobj     instance void [System.Drawing]System.Drawing.Point::.ctor(int32,
                                                                                   int32)
    IL_01b6:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Location(valuetype [System.Drawing]System.Drawing.Point)
    IL_01bb:  ldarg.0
    IL_01bc:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Button WindowsApplication1.Form1::buttonTry
    IL_01c1:  ldstr      "buttonTry"
    IL_01c6:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Name(string)
    IL_01cb:  ldarg.0
    IL_01cc:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Button WindowsApplication1.Form1::buttonTry
    IL_01d1:  ldc.i4.s   75
    IL_01d3:  ldc.i4.s   23
    IL_01d5:  newobj     instance void [System.Drawing]System.Drawing.Size::.ctor(int32,
                                                                                  int32)
    IL_01da:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Size(valuetype [System.Drawing]System.Drawing.Size)
    IL_01df:  ldarg.0
    IL_01e0:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Button WindowsApplication1.Form1::buttonTry
    IL_01e5:  ldc.i4.4
    IL_01e6:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_TabIndex(int32)
    IL_01eb:  ldarg.0
    IL_01ec:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Button WindowsApplication1.Form1::buttonTry
    IL_01f1:  ldstr      "try!"
    IL_01f6:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Text(string)
    IL_01fb:  ldarg.0
    IL_01fc:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Button WindowsApplication1.Form1::buttonTry
    IL_0201:  ldc.i4.1
    IL_0202:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.ButtonBase::set_UseVisualStyleBackColor(bool)
    IL_0207:  ldarg.0
    IL_0208:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Button WindowsApplication1.Form1::buttonTry
    IL_020d:  ldarg.0
    IL_020e:  ldftn      instance void WindowsApplication1.Form1::buttonTry_Click(object,
                                                                                  class [mscorlib]System.EventArgs)
    IL_0214:  newobj     instance void [mscorlib]System.EventHandler::.ctor(object,
                                                                            native int)
    IL_0219:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::add_Click(class [mscorlib]System.EventHandler)
    IL_021e:  ldarg.0
    IL_021f:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Button WindowsApplication1.Form1::buttonAbout
    IL_0224:  ldc.i4     0xb8
    IL_0229:  ldc.i4.s   97
    IL_022b:  newobj     instance void [System.Drawing]System.Drawing.Point::.ctor(int32,
                                                                                   int32)
    IL_0230:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Location(valuetype [System.Drawing]System.Drawing.Point)
    IL_0235:  ldarg.0
    IL_0236:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Button WindowsApplication1.Form1::buttonAbout
    IL_023b:  ldstr      "buttonAbout"
    IL_0240:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Name(string)
    IL_0245:  ldarg.0
    IL_0246:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Button WindowsApplication1.Form1::buttonAbout
    IL_024b:  ldc.i4.s   75
    IL_024d:  ldc.i4.s   23
    IL_024f:  newobj     instance void [System.Drawing]System.Drawing.Size::.ctor(int32,
                                                                                  int32)
    IL_0254:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Size(valuetype [System.Drawing]System.Drawing.Size)
    IL_0259:  ldarg.0
    IL_025a:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Button WindowsApplication1.Form1::buttonAbout
    IL_025f:  ldc.i4.5
    IL_0260:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_TabIndex(int32)
    IL_0265:  ldarg.0
    IL_0266:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Button WindowsApplication1.Form1::buttonAbout
    IL_026b:  ldstr      "about"
    IL_0270:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Text(string)
    IL_0275:  ldarg.0
    IL_0276:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Button WindowsApplication1.Form1::buttonAbout
    IL_027b:  ldc.i4.1
    IL_027c:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.ButtonBase::set_UseVisualStyleBackColor(bool)
    IL_0281:  ldarg.0
    IL_0282:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Button WindowsApplication1.Form1::buttonAbout
    IL_0287:  ldarg.0
    IL_0288:  ldftn      instance void WindowsApplication1.Form1::buttonAbout_Click(object,
                                                                                    class [mscorlib]System.EventArgs)
    IL_028e:  newobj     instance void [mscorlib]System.EventHandler::.ctor(object,
                                                                            native int)
    IL_0293:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::add_Click(class [mscorlib]System.EventHandler)
    IL_0298:  ldarg.0
    IL_0299:  ldfld      class [System.Windows.Forms]System.Windows.Forms.PictureBox WindowsApplication1.Form1::pictureBox1
    IL_029e:  call       class [System.Drawing]System.Drawing.Bitmap WindowsApplication1.Properties.Resources::get_logo()
    IL_02a3:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.PictureBox::set_Image(class [System.Drawing]System.Drawing.Image)
    IL_02a8:  ldarg.0
    IL_02a9:  ldfld      class [System.Windows.Forms]System.Windows.Forms.PictureBox WindowsApplication1.Form1::pictureBox1
    IL_02ae:  ldc.i4.7
    IL_02af:  ldc.i4.0
    IL_02b0:  newobj     instance void [System.Drawing]System.Drawing.Point::.ctor(int32,
                                                                                   int32)
    IL_02b5:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Location(valuetype [System.Drawing]System.Drawing.Point)
    IL_02ba:  ldarg.0
    IL_02bb:  ldfld      class [System.Windows.Forms]System.Windows.Forms.PictureBox WindowsApplication1.Form1::pictureBox1
    IL_02c0:  ldstr      "pictureBox1"
    IL_02c5:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Name(string)
    IL_02ca:  ldarg.0
    IL_02cb:  ldfld      class [System.Windows.Forms]System.Windows.Forms.PictureBox WindowsApplication1.Form1::pictureBox1
    IL_02d0:  ldc.i4     0xfc
    IL_02d5:  ldc.i4.s   50
    IL_02d7:  newobj     instance void [System.Drawing]System.Drawing.Size::.ctor(int32,
                                                                                  int32)
    IL_02dc:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Size(valuetype [System.Drawing]System.Drawing.Size)
    IL_02e1:  ldarg.0
    IL_02e2:  ldfld      class [System.Windows.Forms]System.Windows.Forms.PictureBox WindowsApplication1.Form1::pictureBox1
    IL_02e7:  ldc.i4.1
    IL_02e8:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.PictureBox::set_SizeMode(valuetype [System.Windows.Forms]System.Windows.Forms.PictureBoxSizeMode)
    IL_02ed:  ldarg.0
    IL_02ee:  ldfld      class [System.Windows.Forms]System.Windows.Forms.PictureBox WindowsApplication1.Form1::pictureBox1
    IL_02f3:  ldc.i4.6
    IL_02f4:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.PictureBox::set_TabIndex(int32)
    IL_02f9:  ldarg.0
    IL_02fa:  ldfld      class [System.Windows.Forms]System.Windows.Forms.PictureBox WindowsApplication1.Form1::pictureBox1
    IL_02ff:  ldc.i4.0
    IL_0300:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.PictureBox::set_TabStop(bool)
    IL_0305:  ldarg.0
    IL_0306:  ldc.r4     6.
    IL_030b:  ldc.r4     12.
    IL_0310:  newobj     instance void [System.Drawing]System.Drawing.SizeF::.ctor(float32,
                                                                                   float32)
    IL_0315:  call       instance void [System.Windows.Forms]System.Windows.Forms.ContainerControl::set_AutoScaleDimensions(valuetype [System.Drawing]System.Drawing.SizeF)
    IL_031a:  ldarg.0
    IL_031b:  ldc.i4.1
    IL_031c:  call       instance void [System.Windows.Forms]System.Windows.Forms.ContainerControl::set_AutoScaleMode(valuetype [System.Windows.Forms]System.Windows.Forms.AutoScaleMode)
    IL_0321:  ldarg.0
    IL_0322:  ldc.i4     0x10f
    IL_0327:  ldc.i4     0x84
    IL_032c:  newobj     instance void [System.Drawing]System.Drawing.Size::.ctor(int32,
                                                                                  int32)
    IL_0331:  call       instance void [System.Windows.Forms]System.Windows.Forms.Form::set_ClientSize(valuetype [System.Drawing]System.Drawing.Size)
    IL_0336:  ldarg.0
    IL_0337:  call       instance class [System.Windows.Forms]System.Windows.Forms.Control/ControlCollection [System.Windows.Forms]System.Windows.Forms.Control::get_Controls()
    IL_033c:  ldarg.0
    IL_033d:  ldfld      class [System.Windows.Forms]System.Windows.Forms.PictureBox WindowsApplication1.Form1::pictureBox1
    IL_0342:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control/ControlCollection::Add(class [System.Windows.Forms]System.Windows.Forms.Control)
    IL_0347:  ldarg.0
    IL_0348:  call       instance class [System.Windows.Forms]System.Windows.Forms.Control/ControlCollection [System.Windows.Forms]System.Windows.Forms.Control::get_Controls()
    IL_034d:  ldarg.0
    IL_034e:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Button WindowsApplication1.Form1::buttonAbout
    IL_0353:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control/ControlCollection::Add(class [System.Windows.Forms]System.Windows.Forms.Control)
    IL_0358:  ldarg.0
    IL_0359:  call       instance class [System.Windows.Forms]System.Windows.Forms.Control/ControlCollection [System.Windows.Forms]System.Windows.Forms.Control::get_Controls()
    IL_035e:  ldarg.0
    IL_035f:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Button WindowsApplication1.Form1::buttonTry
    IL_0364:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control/ControlCollection::Add(class [System.Windows.Forms]System.Windows.Forms.Control)
    IL_0369:  ldarg.0
    IL_036a:  call       instance class [System.Windows.Forms]System.Windows.Forms.Control/ControlCollection [System.Windows.Forms]System.Windows.Forms.Control::get_Controls()
    IL_036f:  ldarg.0
    IL_0370:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Label WindowsApplication1.Form1::label2
    IL_0375:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control/ControlCollection::Add(class [System.Windows.Forms]System.Windows.Forms.Control)
    IL_037a:  ldarg.0
    IL_037b:  call       instance class [System.Windows.Forms]System.Windows.Forms.Control/ControlCollection [System.Windows.Forms]System.Windows.Forms.Control::get_Controls()
    IL_0380:  ldarg.0
    IL_0381:  ldfld      class [System.Windows.Forms]System.Windows.Forms.Label WindowsApplication1.Form1::label1
    IL_0386:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control/ControlCollection::Add(class [System.Windows.Forms]System.Windows.Forms.Control)
    IL_038b:  ldarg.0
    IL_038c:  call       instance class [System.Windows.Forms]System.Windows.Forms.Control/ControlCollection [System.Windows.Forms]System.Windows.Forms.Control::get_Controls()
    IL_0391:  ldarg.0
    IL_0392:  ldfld      class [System.Windows.Forms]System.Windows.Forms.TextBox WindowsApplication1.Form1::textBoxKey
    IL_0397:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control/ControlCollection::Add(class [System.Windows.Forms]System.Windows.Forms.Control)
    IL_039c:  ldarg.0
    IL_039d:  call       instance class [System.Windows.Forms]System.Windows.Forms.Control/ControlCollection [System.Windows.Forms]System.Windows.Forms.Control::get_Controls()
    IL_03a2:  ldarg.0
    IL_03a3:  ldfld      class [System.Windows.Forms]System.Windows.Forms.TextBox WindowsApplication1.Form1::textBoxUser
    IL_03a8:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control/ControlCollection::Add(class [System.Windows.Forms]System.Windows.Forms.Control)
    IL_03ad:  ldarg.0
    IL_03ae:  ldc.i4.1
    IL_03af:  call       instance void [System.Windows.Forms]System.Windows.Forms.Form::set_FormBorderStyle(valuetype [System.Windows.Forms]System.Windows.Forms.FormBorderStyle)
    IL_03b4:  ldarg.0
    IL_03b5:  ldc.i4.0
    IL_03b6:  call       instance void [System.Windows.Forms]System.Windows.Forms.Form::set_MaximizeBox(bool)
    IL_03bb:  ldarg.0
    IL_03bc:  ldc.i4.0
    IL_03bd:  call       instance void [System.Windows.Forms]System.Windows.Forms.Form::set_MinimizeBox(bool)
    IL_03c2:  ldarg.0
    IL_03c3:  ldstr      "Form1"
    IL_03c8:  call       instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Name(string)
    IL_03cd:  ldarg.0
    IL_03ce:  ldstr      "NE365 .Net CrackMe 2.1"
    IL_03d3:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.Control::set_Text(string)
    IL_03d8:  ldarg.0
    IL_03d9:  ldarg.0
    IL_03da:  ldftn      instance void WindowsApplication1.Form1::Form1_Load(object,
                                                                             class [mscorlib]System.EventArgs)
    IL_03e0:  newobj     instance void [mscorlib]System.EventHandler::.ctor(object,
                                                                            native int)
    IL_03e5:  call       instance void [System.Windows.Forms]System.Windows.Forms.Form::add_Load(class [mscorlib]System.EventHandler)
    IL_03ea:  ldarg.0
    IL_03eb:  ldfld      class [System.Windows.Forms]System.Windows.Forms.PictureBox WindowsApplication1.Form1::pictureBox1
    IL_03f0:  callvirt   instance void [System]System.ComponentModel.ISupportInitialize::EndInit()
    IL_03f5:  ldarg.0
    IL_03f6:  ldc.i4.0
    IL_03f7:  call       instance void [System.Windows.Forms]System.Windows.Forms.Control::ResumeLayout(bool)
    IL_03fc:  ldarg.0
    IL_03fd:  call       instance void [System.Windows.Forms]System.Windows.Forms.Control::PerformLayout()
    IL_0402:  ret
  } // end of method Form1::InitializeComponent

  .method public hidebysig specialname rtspecialname 
          instance void  .ctor() cil managed
  {
    // Code size       13 (0xd)
    .maxstack  8
    IL_0000:  ldarg.0
    IL_0001:  call       instance void [System.Windows.Forms]System.Windows.Forms.Form::.ctor()
    IL_0006:  ldarg.0
    IL_0007:  call       instance void WindowsApplication1.Form1::InitializeComponent()
    IL_000c:  ret
  } // end of method Form1::.ctor

  .method private hidebysig instance void 
          buttonTry_Click(object sender,
                          class [mscorlib]System.EventArgs e) cil managed
  {
    // Code size       77 (0x4d)
    .maxstack  2
    IL_0000:  ldarg.0
    IL_0001:  ldfld      class [System.Windows.Forms]System.Windows.Forms.TextBox WindowsApplication1.Form1::textBoxUser
    IL_0006:  callvirt   instance string [System.Windows.Forms]System.Windows.Forms.Control::get_Text()
    IL_000b:  callvirt   instance int32 [mscorlib]System.String::get_Length()
    IL_0010:  brtrue.s   IL_0023

    IL_0012:  ldstr      "please enter user name"
    IL_0017:  ldstr      "error"
    IL_001c:  call       valuetype [System.Windows.Forms]System.Windows.Forms.DialogResult [System.Windows.Forms]System.Windows.Forms.MessageBox::Show(string,
                                                                                                                                                       string)
    IL_0021:  pop
    IL_0022:  ret

    IL_0023:  ldarg.0
    IL_0024:  ldfld      class [System.Windows.Forms]System.Windows.Forms.TextBox WindowsApplication1.Form1::textBoxKey
    IL_0029:  callvirt   instance string [System.Windows.Forms]System.Windows.Forms.Control::get_Text()
    IL_002e:  callvirt   instance int32 [mscorlib]System.String::get_Length()
    IL_0033:  brtrue.s   IL_0046

    IL_0035:  ldstr      "please enter key code"
    IL_003a:  ldstr      "error"
    IL_003f:  call       valuetype [System.Windows.Forms]System.Windows.Forms.DialogResult [System.Windows.Forms]System.Windows.Forms.MessageBox::Show(string,
                                                                                                                                                       string)
    IL_0044:  pop
    IL_0045:  ret

    IL_0046:  ldarg.0
    IL_0047:  call       instance void WindowsApplication1.Form1::crypt1()
    IL_004c:  ret
  } // end of method Form1::buttonTry_Click

  .method private hidebysig instance void 
          buttonAbout_Click(object sender,
                            class [mscorlib]System.EventArgs e) cil managed
  {
    // Code size       19 (0x13)
    .maxstack  2
    .locals init (string V_0)
    IL_0000:  ldstr      "NE365 .Net CrackMe 2.1\ncode by:tankaiha\nseries 2 a"
    + "re little difficult, good luck!\nif you get the key, please let me know."
    + "\ncome and visit http://vxer.cn"
    IL_0005:  stloc.0
    IL_0006:  ldloc.0
    IL_0007:  ldstr      "about"
    IL_000c:  call       valuetype [System.Windows.Forms]System.Windows.Forms.DialogResult [System.Windows.Forms]System.Windows.Forms.MessageBox::Show(string,
                                                                                                                                                       string)
    IL_0011:  pop
    IL_0012:  ret
  } // end of method Form1::buttonAbout_Click

  .method private hidebysig instance void 
          crypt1() cil managed
  {
    // Code size       163 (0xa3)
    .maxstack  4
    .locals init (string V_0,
             int32 V_1,
             int32 V_2)
    IL_0000:  ldarg.0
    IL_0001:  call       class [mscorlib]System.Text.Encoding [mscorlib]System.Text.Encoding::get_UTF8()
    IL_0006:  ldarg.0
    IL_0007:  ldfld      class [System.Windows.Forms]System.Windows.Forms.TextBox WindowsApplication1.Form1::textBoxUser
    IL_000c:  callvirt   instance string [System.Windows.Forms]System.Windows.Forms.Control::get_Text()
    IL_0011:  callvirt   instance uint8[] [mscorlib]System.Text.Encoding::GetBytes(string)
    IL_0016:  stfld      uint8[] WindowsApplication1.Form1::strName
    IL_001b:  ldc.i4.0
    IL_001c:  stloc.1
    IL_001d:  br.s       IL_0037

    IL_001f:  ldarg.0
    IL_0020:  ldfld      uint8[] WindowsApplication1.Form1::strName
    IL_0025:  ldloc.1
    IL_0026:  ldarg.0
    IL_0027:  ldfld      uint8[] WindowsApplication1.Form1::strName
    IL_002c:  ldloc.1
    IL_002d:  ldelem.u1
    IL_002e:  ldc.i4.s   67
    IL_0030:  or
    IL_0031:  conv.u1
    IL_0032:  stelem.i1
    IL_0033:  ldloc.1
    IL_0034:  ldc.i4.1
    IL_0035:  add
    IL_0036:  stloc.1
    IL_0037:  ldloc.1
    IL_0038:  ldarg.0
    IL_0039:  ldfld      uint8[] WindowsApplication1.Form1::strName
    IL_003e:  ldlen
    IL_003f:  conv.i4
    IL_0040:  blt.s      IL_001f

    IL_0042:  ldc.i4.0
    IL_0043:  stloc.2
    IL_0044:  br.s       IL_0067

    IL_0046:  ldarg.0
    IL_0047:  ldfld      uint8[] WindowsApplication1.Form1::strName
    IL_004c:  ldloc.2
    IL_004d:  ldelema    [mscorlib]System.Byte
    IL_0052:  dup
    IL_0053:  ldobj      [mscorlib]System.Byte
    IL_0058:  ldloc.2
    IL_0059:  ldc.i4.s   31
    IL_005b:  and
    IL_005c:  shr
    IL_005d:  conv.u1
    IL_005e:  stobj      [mscorlib]System.Byte
    IL_0063:  ldloc.2
    IL_0064:  ldc.i4.1
    IL_0065:  add
    IL_0066:  stloc.2
    IL_0067:  ldloc.2
    IL_0068:  ldarg.0
    IL_0069:  ldfld      uint8[] WindowsApplication1.Form1::strName
    IL_006e:  ldlen
    IL_006f:  conv.i4
    IL_0070:  blt.s      IL_0046

    IL_0072:  ldarg.0
    IL_0073:  ldarg.0
    IL_0074:  ldfld      uint8[] WindowsApplication1.Form1::strName
    IL_0079:  call       instance string WindowsApplication1.Form1::cryptcal(uint8[])
    IL_007e:  stloc.0
    IL_007f:  ldloc.0
    IL_0080:  ldarg.0
    IL_0081:  ldfld      class [System.Windows.Forms]System.Windows.Forms.TextBox WindowsApplication1.Form1::textBoxKey
    IL_0086:  callvirt   instance string [System.Windows.Forms]System.Windows.Forms.Control::get_Text()
    IL_008b:  call       bool [mscorlib]System.String::op_Equality(string,
                                                                   string)
    IL_0090:  pop//brfalse.s  IL_00a2

    IL_0092:  ldstr      "you get it "
    IL_0097:  ldstr      "congratulations"
    IL_009c:  call       valuetype [System.Windows.Forms]System.Windows.Forms.DialogResult [System.Windows.Forms]System.Windows.Forms.MessageBox::Show(string,
                                                                                                                                                       string)
    IL_00a1:  pop
    IL_00a2:  ret
  } // end of method Form1::crypt1

  .method private hidebysig instance string 
          cryptcal(uint8[] textToCal) cil managed
  {
    // Code size       9 (0x9)
    .maxstack  1
    .locals init (string V_0)
    IL_0000:  ldarg.1
    IL_0001:  call       string [mscorlib]System.Convert::ToBase64String(uint8[])
    IL_0006:  stloc.0
    IL_0007:  ldloc.0
    IL_0008:  ret
  } // end of method Form1::cryptcal

  .method private hidebysig instance void 
          Form1_Load(object sender,
                     class [mscorlib]System.EventArgs e) cil managed
  {
    // Code size       1 (0x1)
    .maxstack  8
    IL_0000:  ret
  } // end of method Form1::Form1_Load

} // end of class WindowsApplication1.Form1

.class private auto ansi sealed beforefieldinit WindowsApplication1.Properties.Settings
       extends [System]System.Configuration.ApplicationSettingsBase
{
  .custom instance void [System]System.CodeDom.Compiler.GeneratedCodeAttribute::.ctor(string,
                                                                                      string) = ( 01 00 4B 4D 69 63 72 6F 73 6F 66 74 2E 56 69 73   // ..KMicrosoft.Vis
                                                                                                  75 61 6C 53 74 75 64 69 6F 2E 45 64 69 74 6F 72   // ualStudio.Editor
                                                                                                  73 2E 53 65 74 74 69 6E 67 73 44 65 73 69 67 6E   // s.SettingsDesign
                                                                                                  65 72 2E 53 65 74 74 69 6E 67 73 53 69 6E 67 6C   // er.SettingsSingl
                                                                                                  65 46 69 6C 65 47 65 6E 65 72 61 74 6F 72 07 38   // eFileGenerator.8
                                                                                                  2E 30 2E 30 2E 30 00 00 )                         // .0.0.0..
  .custom instance void [mscorlib]System.Runtime.CompilerServices.CompilerGeneratedAttribute::.ctor() = ( 01 00 00 00 ) 
  .field private static class WindowsApplication1.Properties.Settings defaultInstance
  .method public hidebysig specialname static 
          class WindowsApplication1.Properties.Settings 
          get_Default() cil managed
  {
    // Code size       6 (0x6)
    .maxstack  8
    IL_0000:  ldsfld     class WindowsApplication1.Properties.Settings WindowsApplication1.Properties.Settings::defaultInstance
    IL_0005:  ret
  } // end of method Settings::get_Default

  .method public hidebysig specialname rtspecialname 
          instance void  .ctor() cil managed
  {
    // Code size       7 (0x7)
    .maxstack  8
    IL_0000:  ldarg.0
    IL_0001:  call       instance void [System]System.Configuration.ApplicationSettingsBase::.ctor()
    IL_0006:  ret
  } // end of method Settings::.ctor

  .method private hidebysig specialname rtspecialname static 
          void  .cctor() cil managed
  {
    // Code size       21 (0x15)
    .maxstack  8
    IL_0000:  newobj     instance void WindowsApplication1.Properties.Settings::.ctor()
    IL_0005:  call       class [System]System.Configuration.SettingsBase [System]System.Configuration.SettingsBase::Synchronized(class [System]System.Configuration.SettingsBase)
    IL_000a:  castclass  WindowsApplication1.Properties.Settings
    IL_000f:  stsfld     class WindowsApplication1.Properties.Settings WindowsApplication1.Properties.Settings::defaultInstance
    IL_0014:  ret
  } // end of method Settings::.cctor

  .property class WindowsApplication1.Properties.Settings
          Default()
  {
    .get class WindowsApplication1.Properties.Settings WindowsApplication1.Properties.Settings::get_Default()
  } // end of property Settings::Default
} // end of class WindowsApplication1.Properties.Settings

.class private auto ansi beforefieldinit WindowsApplication1.Properties.Resources
       extends [mscorlib]System.Object
{
  .custom instance void [mscorlib]System.Runtime.CompilerServices.CompilerGeneratedAttribute::.ctor() = ( 01 00 00 00 ) 
  .custom instance void [System]System.CodeDom.Compiler.GeneratedCodeAttribute::.ctor(string,
                                                                                      string) = ( 01 00 33 53 79 73 74 65 6D 2E 52 65 73 6F 75 72   // ..3System.Resour
                                                                                                  63 65 73 2E 54 6F 6F 6C 73 2E 53 74 72 6F 6E 67   // ces.Tools.Strong
                                                                                                  6C 79 54 79 70 65 64 52 65 73 6F 75 72 63 65 42   // lyTypedResourceB
                                                                                                  75 69 6C 64 65 72 07 32 2E 30 2E 30 2E 30 00 00 ) // uilder.2.0.0.0..
  .custom instance void [mscorlib]System.Diagnostics.DebuggerNonUserCodeAttribute::.ctor() = ( 01 00 00 00 ) 
  .field private static class [mscorlib]System.Resources.ResourceManager resourceMan
  .field private static class [mscorlib]System.Globalization.CultureInfo resourceCulture
  .method assembly hidebysig specialname rtspecialname 
          instance void  .ctor() cil managed
  {
    // Code size       7 (0x7)
    .maxstack  8
    IL_0000:  ldarg.0
    IL_0001:  call       instance void [mscorlib]System.Object::.ctor()
    IL_0006:  ret
  } // end of method Resources::.ctor

  .method assembly hidebysig specialname static 
          class [mscorlib]System.Resources.ResourceManager 
          get_ResourceManager() cil managed
  {
    // Code size       51 (0x33)
    .maxstack  3
    .locals init (class [mscorlib]System.Resources.ResourceManager V_0)
    IL_0000:  ldsfld     class [mscorlib]System.Resources.ResourceManager WindowsApplication1.Properties.Resources::resourceMan
    IL_0005:  ldnull
    IL_0006:  call       bool [mscorlib]System.Object::ReferenceEquals(object,
                                                                       object)
    IL_000b:  brfalse.s  IL_002d

    IL_000d:  ldstr      "WindowsApplication1.Properties.Resources"
    IL_0012:  ldtoken    WindowsApplication1.Properties.Resources
    IL_0017:  call       class [mscorlib]System.Type [mscorlib]System.Type::GetTypeFromHandle(valuetype [mscorlib]System.RuntimeTypeHandle)
    IL_001c:  callvirt   instance class [mscorlib]System.Reflection.Assembly [mscorlib]System.Type::get_Assembly()
    IL_0021:  newobj     instance void [mscorlib]System.Resources.ResourceManager::.ctor(string,
                                                                                         class [mscorlib]System.Reflection.Assembly)
    IL_0026:  stloc.0
    IL_0027:  ldloc.0
    IL_0028:  stsfld     class [mscorlib]System.Resources.ResourceManager WindowsApplication1.Properties.Resources::resourceMan
    IL_002d:  ldsfld     class [mscorlib]System.Resources.ResourceManager WindowsApplication1.Properties.Resources::resourceMan
    IL_0032:  ret
  } // end of method Resources::get_ResourceManager

  .method assembly hidebysig specialname static 
          class [mscorlib]System.Globalization.CultureInfo 
          get_Culture() cil managed
  {
    // Code size       6 (0x6)
    .maxstack  8
    IL_0000:  ldsfld     class [mscorlib]System.Globalization.CultureInfo WindowsApplication1.Properties.Resources::resourceCulture
    IL_0005:  ret
  } // end of method Resources::get_Culture

  .method assembly hidebysig specialname static 
          void  set_Culture(class [mscorlib]System.Globalization.CultureInfo 'value') cil managed
  {
    // Code size       7 (0x7)
    .maxstack  8
    IL_0000:  ldarg.0
    IL_0001:  stsfld     class [mscorlib]System.Globalization.CultureInfo WindowsApplication1.Properties.Resources::resourceCulture
    IL_0006:  ret
  } // end of method Resources::set_Culture

  .method assembly hidebysig specialname static 
          class [System.Drawing]System.Drawing.Bitmap 
          get_logo() cil managed
  {
    // Code size       28 (0x1c)
    .maxstack  3
    .locals init (object V_0)
    IL_0000:  call       class [mscorlib]System.Resources.ResourceManager WindowsApplication1.Properties.Resources::get_ResourceManager()
    IL_0005:  ldstr      "logo"
    IL_000a:  ldsfld     class [mscorlib]System.Globalization.CultureInfo WindowsApplication1.Properties.Resources::resourceCulture
    IL_000f:  callvirt   instance object [mscorlib]System.Resources.ResourceManager::GetObject(string,
                                                                                               class [mscorlib]System.Globalization.CultureInfo)
    IL_0014:  stloc.0
    IL_0015:  ldloc.0
    IL_0016:  castclass  [System.Drawing]System.Drawing.Bitmap
    IL_001b:  ret
  } // end of method Resources::get_logo

  .property class [mscorlib]System.Resources.ResourceManager
          ResourceManager()
  {
    .custom instance void [System]System.ComponentModel.EditorBrowsableAttribute::.ctor(valuetype [System]System.ComponentModel.EditorBrowsableState) = ( 01 00 02 00 00 00 00 00 ) 
    .get class [mscorlib]System.Resources.ResourceManager WindowsApplication1.Properties.Resources::get_ResourceManager()
  } // end of property Resources::ResourceManager
  .property class [mscorlib]System.Globalization.CultureInfo
          Culture()
  {
    .custom instance void [System]System.ComponentModel.EditorBrowsableAttribute::.ctor(valuetype [System]System.ComponentModel.EditorBrowsableState) = ( 01 00 02 00 00 00 00 00 ) 
    .get class [mscorlib]System.Globalization.CultureInfo WindowsApplication1.Properties.Resources::get_Culture()
    .set void WindowsApplication1.Properties.Resources::set_Culture(class [mscorlib]System.Globalization.CultureInfo)
  } // end of property Resources::Culture
  .property class [System.Drawing]System.Drawing.Bitmap
          logo()
  {
    .get class [System.Drawing]System.Drawing.Bitmap WindowsApplication1.Properties.Resources::get_logo()
  } // end of property Resources::logo
} // end of class WindowsApplication1.Properties.Resources


// =============================================================

// *********** DISASSEMBLY COMPLETE ***********************
// WARNING: Created Win32 resource file E:\1.res
