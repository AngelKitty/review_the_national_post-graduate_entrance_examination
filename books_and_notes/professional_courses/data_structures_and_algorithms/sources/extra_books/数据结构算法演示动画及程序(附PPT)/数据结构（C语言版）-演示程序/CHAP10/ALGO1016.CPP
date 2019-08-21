void Collect(SLList &L, int i, ArrType f, ArrType e) {  // 算法10.16
  // 本算法按keys[i]自小至大地将f[0..RADIX-1]所指各子表依次链接成
  // 一个链表，e[0..RADIX-1]为各子表的尾指针
  int j,t;
  for (j=0; !f[j]; j++);  // 找第一个非空子表，succ为求后继函数: ++
  L.r[0].next = f[j];  // L.r[0].next指向第一个非空子表中第一个结点
  t = e[j];
  while (j<RADIX) {
    for (j=j+1; j<RADIX && !f[j]; j++);       // 找下一个非空子表
    if (j<RADIX) // 链接两个非空子表
      { L.r[t].next = f[j];  t = e[j]; }
  }
  L.r[t].next = 0;   // t指向最后一个非空子表中的最后一个结点
} // Collect
