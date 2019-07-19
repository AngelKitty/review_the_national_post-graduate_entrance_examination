//Imports.idc 列出当前程序的输入函数
//(c)  www.PEDIY.com 2000-2008

#include <idc.idc>

static GetImportSeg()
{
    auto ea, next, name;
    ea = FirstSeg();
    next = ea;
    while ( (next = NextSeg(next)) != -1) {
        name = SegName(next);
        if ( substr( name, 0, 6 ) == ".idata" ) break;
   }
   return next;
}

static main()
{
    auto BytePtr, EndImports;
    BytePtr = SegStart( GetImportSeg() );
    EndImports = SegEnd( BytePtr );
    Message(" \n" + "Parsing Import Table...\n");
    while ( BytePtr < EndImports ) {
        if (LineA(BytePtr, 1) != "") Message("\n" + "____" + LineA(BytePtr,1) + "____" + "\n");
        if (Name(BytePtr)!= "") Message(Name(BytePtr) + "\n");
        BytePtr = NextAddr(BytePtr);
    }
    Message("\n" + "Import Table Parsing Complete\n");
}
