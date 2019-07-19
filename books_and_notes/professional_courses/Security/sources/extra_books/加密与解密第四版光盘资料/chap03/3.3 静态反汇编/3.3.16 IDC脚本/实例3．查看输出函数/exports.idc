//exports.idc 
//(c)  www.PEDIY.com 2000-2008

#include <idc.idc>

static main()
{
    auto x, ord, ea;
    Message("\n Program Entry Points: \n \n");
	for ( x=0; x<= GetEntryPointQty(); x = x+1){  //GetEntryPointQty()得到入口点个数
		ord =  GetEntryOrdinal( x );          //得到该入口点的序列数
		ea = GetEntryPoint( ord );
        Message( Name( ea ) + ":  Ordinal " + ltoa( ord,16 ) + " at offset " + ltoa( ea, 16) + "\n");
	}
    Message("\n" + "Export Parsing Complete\n");
}
