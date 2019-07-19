.assembly extern mscorlib{}
.assembly sample1044{}
.module sample1044.exe
.imagebase 0x00400000	//可省略
.subsystem 0x0003	//可省略

.namespace tankaiha.sample1044
{
	.class private auto ansi beforefieldinit class1 extends [mscorlib]System.Object
	{
	  .method public hidebysig static void  Main() cil managed
	  {
	    .entrypoint	  
	    .maxstack  1
	    .locals init (class [mscorlib]System.Reflection.Assembly V_0)
	    IL_0000:  nop
	    IL_0001:  call       class [mscorlib]System.Reflection.Assembly [mscorlib]System.Reflection.Assembly::GetEntryAssembly()
	    IL_0006:  stloc.0
	    IL_0007:  ldloc.0
	    IL_0008:  callvirt   instance string [mscorlib]System.Object::ToString()
	    IL_000d:  call       void [mscorlib]System.Console::WriteLine(string)
	    IL_0012:  nop
	    IL_0013:  ret
	  }

	  .method public hidebysig specialname rtspecialname instance void  .ctor() cil managed
	  {
	    .maxstack  8
	    IL_0000:  ldarg.0
	    IL_0001:  call       instance void [mscorlib]System.Object::.ctor()
	    IL_0006:  ret
	  }

	} 
}