void conversion (int Num) {  // 算法3.1
    // 对于输入的任意一个非负十进制整数，打印输出与其等值的八进制数
    ElemType e;  
    SqStack S;
    InitStack(S);      // 构造空栈
    while (Num) {
        Push(S, Num % 8);
        Num = Num/8;
    }
    while (!StackEmpty(S)) {
        Pop(S,e);
        printf ("%d", e);
    }
    printf("\n");
} // conversion
