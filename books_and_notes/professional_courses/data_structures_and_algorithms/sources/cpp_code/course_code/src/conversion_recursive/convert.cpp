/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "../conversion/convert.h"

void convert ( Stack<char>& S, __int64 n, int base ) { //十进制正整数n到base进制的转换（递归版）
   static char digit[] //0 < n, 1 < base <= 16，新进制下的数位符号，可视base取值范围适当扩充
   = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
   if ( 0 < n ) { //在尚有余数之前，反复地
      S.push ( digit[n % base] ); //逆向记录当前最低位，再
      convert ( S, n / base, base ); //通过递归得到所有更高位
   }
} //新进制下由高到低的各数位，自顶而下保存于栈S中