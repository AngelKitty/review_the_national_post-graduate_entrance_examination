/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "rpn.h"

void readNumber ( char*& p, Stack<float>& stk ) { //将起始于p的子串解析为数值，并存入操作数栈
   stk.push ( ( float ) ( *p - '0' ) ); //当前数位对应的数值进栈
   while ( isdigit ( * ( ++p ) ) ) //只要后续还有紧邻的数字（即多位整数的情况），则
      stk.push ( stk.pop() * 10 + ( *p - '0' ) ); //弹出原操作数并追加新数位后，新数值重新入栈
   if ( '.' != *p ) return; //此后非小数点，则意味着当前操作数解析完成
   float fraction = 1; //否则，意味着还有小数部分
   while ( isdigit ( * ( ++p ) ) ) //逐位加入
      stk.push ( stk.pop() + ( *p - '0' ) * ( fraction /= 10 ) ); //小数部分
}
