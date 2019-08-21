void GetPowerSet(int i, List A, List &B) {  // 算法6.15
   // 线性表A表示集合A，线性表B表示幂集ρ(A)的一个元素。
   // 局部量k为进入函数时表B的当前长度。
   // 第一次调用本函数时，B为空表，i＝1。
   ElemType x;
   int k;
   if (i > ListLength(A)) Output(B); // 输出当前B值，即ρ(A)的一个元素
   else { 
      GetElem(A, i, x);        k = ListLength(B);
      ListInsert(B, k+1, x);   GetPowerSet(i+1, A, B);
      ListDelete(B, k+1, x);   GetPowerSet(i+1, A, B);
   }
} // GetPowerSet
