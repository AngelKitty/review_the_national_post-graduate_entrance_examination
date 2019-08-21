void Distribute(SLList &L, int i, ArrType &f, ArrType &e) {  
  // 算法10.15
  // 静态链表L的r域中记录已按(keys[0],...,keys[i-1])有序，
  // 本算法按第i个关键字keys[i]建立RADIX个子表，
  // 使同一子表中记录的keys[i]相同。f[0..RADIX-1]和e[0..RADIX-1]
  // 分别指向各子表中第一个和最后一个记录。
  int j, p;
  for (j=0; j<RADIX; ++j) f[j] = 0;     // 各子表初始化为空表
  for (p=L.r[0].next;  p;  p=L.r[p].next) {
    j = L.r[p].keys[i]-'0';  // 将记录中第i个关键字映射到[0..RADIX-1]，
    if (!f[j]) f[j] = p;
    else L.r[e[j]].next = p;
    e[j] = p;                // 将p所指的结点插入第j个子表中
  }
} // Distribute
