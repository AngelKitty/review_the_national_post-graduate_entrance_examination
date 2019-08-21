/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "rpn.h"

Operator optr2rank ( char op ) { //由运算符转译出编号
   switch ( op ) {
      case '+' : return ADD; //加
      case '-' : return SUB; //减
      case '*' : return MUL; //乘
      case '/' : return DIV; //除
      case '^' : return POW; //乘方
      case '!' : return FAC; //阶乘
      case '(' : return L_P; //左括号
      case ')' : return R_P; //右括号
      case '\0': return EOE; //起始符与终止符
      default  : exit ( -1 ); //未知运算符
   }
}

char orderBetween ( char op1, char op2 ) //比较两个运算符之间的优先级
{ return pri[optr2rank ( op1 ) ][optr2rank ( op2 ) ]; }
