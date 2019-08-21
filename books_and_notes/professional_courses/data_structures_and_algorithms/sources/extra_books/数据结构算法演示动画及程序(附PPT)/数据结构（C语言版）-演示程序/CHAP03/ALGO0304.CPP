#define OPSETSIZE 7
unsigned char Prior[7][7] = {     // 表3.1  算符间的优先关系
  	  '>','>','<','<','<','>','>',
	  '>','>','<','<','<','>','>',
	  '>','>','>','>','<','>','>',
	  '>','>','>','>','<','>','>',	
	  '<','<','<','<','<','=',' ',
	  '>','>','>','>',' ','>','>',
	  '<','<','<','<','<',' ','='
};		
float Operate(float a, unsigned char theta, float b);
char OPSET[OPSETSIZE]={'+' , '-' , '*' , '/' ,'(' , ')' , '#'};
Status In(char Test,char* TestOp);
char precede(char Aop, char Bop);
		
float EvaluateExpression(char* MyExpression) {  // 算法3.4
   // 算术表达式求值的算符优先算法。
   // 设OPTR和OPND分别为运算符栈和运算数栈，OP为运算符集合。
   StackChar  OPTR;    // 运算符栈，字符元素
   StackFloat OPND;    // 运算数栈，实数元素
   char TempData[20];
   float Data,a,b;
   char theta,*c,x,Dr[2];
   
   InitStack (OPTR);
   Push (OPTR, '#');
   InitStack (OPND);
   c = MyExpression;
   strcpy(TempData,"\0");
   while (*c!= '#' || GetTop(OPTR)!= '#') {
      if (!In(*c, OPSET)) {
      	 Dr[0]=*c;
      	 Dr[1]='\0';
         strcat(TempData,Dr);
         c++;
         if(In(*c,OPSET)) {
            Data=(float)atof(TempData);
            Push(OPND, Data);
            strcpy(TempData,"\0");
         }
      } else {   // 不是运算符则进栈
         switch (precede(GetTop(OPTR), *c)) { 
            case '<':   // 栈顶元素优先权低
                 Push(OPTR, *c);  
                 c++;
                 break;
            case '=':   // 脱括号并接收下一字符
                 Pop(OPTR, x);   
                 c++;
                 break;
            case '>':   // 退栈并将运算结果入栈
                 Pop(OPTR, theta);
                 Pop(OPND, b);  
                 Pop(OPND, a);                      
                 Push(OPND, Operate(a, theta, b)); 
                 break;
         } // switch
      }
   } // while
   return GetTop(OPND);
} // EvaluateExpression

float Operate(float a,unsigned char theta, float b) {
   switch(theta) {
      case '+': return a+b;
      case '-': return a-b;
      case '*': return a*b;
      case '/': return a/b;
      default : return 0;
   } 
}	

Status In(char Test,char* TestOp) {
   bool Find=false;
   for (int i=0; i< OPSETSIZE; i++) {
      if (Test == TestOp[i]) Find= true;
   }
   return Find;
}


int ReturnOpOrd(char op,char* TestOp) {
   int i;
   for(i=0; i< OPSETSIZE; i++) {
      if (op == TestOp[i]) return i;
   }
   return 0;
}

char precede(char Aop, char Bop) {
   return Prior[ReturnOpOrd(Aop,OPSET)][ReturnOpOrd(Bop,OPSET)];
}
