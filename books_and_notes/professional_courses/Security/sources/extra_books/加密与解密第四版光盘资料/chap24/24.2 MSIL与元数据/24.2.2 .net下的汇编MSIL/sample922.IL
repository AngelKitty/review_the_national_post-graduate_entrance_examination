.assembly extern mscorlib{}
.assembly sample1022{}
.module	  sample1022.exe

.class public auto ansi class1 extends System.Object
{
    .field private static int32 sum
    .method public static void addTwoInts()
    {
	.entrypoint
	.locals init(int32 val)
	ldc.i4.1
	stloc.0	//也可写成stloc val
	ldc.i4 10
	ldloc.0 //也可写成ldloc val
	add
	stsfld int32 class1::sum
	ldsfld int32 class1::sum
	ldstr "1+10="
	call void [mscorlib]System.Console::WriteLine(string)
	call void [mscorlib]System.Console::WriteLine(int32)
	nop
	ret
    }
}