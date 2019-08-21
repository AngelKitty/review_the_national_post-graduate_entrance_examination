void ShellSort(SqList &L, int dlta[], int t) {  // 算法10.5
   // 按增量序列dlta[0..t-1]对顺序表L作希尔排序。
   for (int k=0; k<t; ++k)
      ShellInsert(L, dlta[k]);  // 一趟增量为dlta[k]的插入排序
} // ShellSort
