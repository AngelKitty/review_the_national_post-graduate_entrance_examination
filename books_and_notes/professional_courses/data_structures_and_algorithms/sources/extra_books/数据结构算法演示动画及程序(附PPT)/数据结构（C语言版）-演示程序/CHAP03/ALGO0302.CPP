void LineEdit() {  // 算法3.2
  //利用字符栈S，从终端接收一行并传送至调用过程的数据区。
  char ch,*temp;
  SqStack S;
  InitStack(S);       //构造空栈S
  printf("请输入一行(#:退格；@:清行):\n");
  ch = getchar();     //从终端接收第一个字符
  while (ch != EOF) { //EOF为全文结束符
    while (ch != EOF && ch != '\n') {
      switch (ch) {
        case '#': Pop(S, ch);    break;  // 仅当栈非空时退栈
        case '@': ClearStack(S); break;  // 重置S为空栈
        default : Push(S, ch);   break;  // 有效字符进栈,未考虑栈满情形
      }
      ch = getchar();  // 从终端接收下一个字符
    }	
    temp=S.base;
    while(temp!=S.top) {
      printf("%c",*temp);	      	
      ++temp;
    }		
    // 将从栈底到栈顶的栈内字符传送至调用过程的数据区；
    ClearStack(S);      // 重置S为空栈
    printf("\n");
    if (ch != EOF) {
      printf("请输入一行(#:退格；@:清行):\n");
      ch = getchar();
    }
  }
  DestroyStack(S);
}
