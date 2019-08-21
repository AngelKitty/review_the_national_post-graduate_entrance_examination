void InsertSort(SqList &L) {  // 算法10.1
  // 对顺序表L作直接插入排序。
  int i,j;
  for (i=2; i<=L.length; ++i)
    if (LT(L.r[i].key, L.r[i-1].key)) { 
      // "<"时，需将L.r[i]插入有序子表
      L.r[0] = L.r[i];                 // 复制为哨兵
      for (j=i-1;  LT(L.r[0].key, L.r[j].key);  --j)
        L.r[j+1] = L.r[j];             // 记录后移
      L.r[j+1] = L.r[0];               // 插入到正确位置
    }
} // InsertSort
