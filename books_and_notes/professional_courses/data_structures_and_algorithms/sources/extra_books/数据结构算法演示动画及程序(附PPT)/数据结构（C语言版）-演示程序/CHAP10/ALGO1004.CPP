void ShellInsert(SqList &L, int dk) {  // 算法10.4
  // 对顺序表L作一趟希尔插入排序。本算法对算法10.1作了以下修改：
  //     1. 前后记录位置的增量是dk，而不是1；
  //     2. r[0]只是暂存单元，不是哨兵。当j<=0时，插入位置已找到。
  int i,j;
  for (i=dk+1; i<=L.length; ++i)
    if (LT(L.r[i].key, L.r[i-dk].key)) { // 需将L.r[i]插入有序增量子表
      L.r[0] = L.r[i];                   // 暂存在L.r[0]
      for (j=i-dk; j>0 && LT(L.r[0].key, L.r[j].key); j-=dk)
        L.r[j+dk] = L.r[j];              // 记录后移，查找插入位置
      L.r[j+dk] = L.r[0];                // 插入
    }
} // ShellInsert
