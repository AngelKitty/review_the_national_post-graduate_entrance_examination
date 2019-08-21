/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "RPN.h"

float evaluate ( char* S, char*& RPN ) { //对（已剔除白空格的）表达式S求值，并转换为逆波兰式RPN
   Stack<float> opnd; Stack<char> optr; //运算数栈、运算符栈 /*DSA*/任何时刻，其中每对相邻元素之间均大小一致
   /*DSA*/ char* expr = S;
   optr.push ( '\0' ); //尾哨兵'\0'也作为头哨兵首先入栈
   while ( !optr.empty() ) { //在运算符栈非空之前，逐个处理表达式中各字符
      if ( isdigit ( *S ) ) { //若当前字符为操作数，则
         readNumber ( S, opnd ); append ( RPN, opnd.top() ); //读入操作数，并将其接至RPN末尾
      } else //若当前字符为运算符，则
         switch ( orderBetween ( optr.top(), *S ) ) { //视其与栈顶运算符之间优先级高低分别处理
            case '<': //栈顶运算符优先级更低时
               optr.push ( *S ); S++; //计算推迟，当前运算符进栈
               break;
            case '=': //优先级相等（当前运算符为右括号或者尾部哨兵'\0'）时
               optr.pop(); S++; //脱括号并接收下一个字符
               break;
            case '>': { //栈顶运算符优先级更高时，可实施相应的计算，并将结果重新入栈
               char op = optr.pop(); append ( RPN, op ); //栈顶运算符出栈并续接至RPN末尾
               if ( '!' == op ) { //若属于一元运算符
                  float pOpnd = opnd.pop(); //只需取出一个操作数，并
                  opnd.push ( calcu ( op, pOpnd ) ); //实施一元计算，结果入栈
               } else { //对于其它（二元）运算符
                  float pOpnd2 = opnd.pop(), pOpnd1 = opnd.pop(); //取出后、前操作数 /*DSA*/提问：可否省去两个临时变量？
                  opnd.push ( calcu ( pOpnd1, op, pOpnd2 ) ); //实施二元计算，结果入栈
               }
               break;
            }
            default : exit ( -1 ); //逢语法错误，不做处理直接退出
         }//switch
      /*DSA*/displayProgress ( expr, S, opnd, optr, RPN );
   }//while
   return opnd.pop(); //弹出并返回最后的计算结果
}