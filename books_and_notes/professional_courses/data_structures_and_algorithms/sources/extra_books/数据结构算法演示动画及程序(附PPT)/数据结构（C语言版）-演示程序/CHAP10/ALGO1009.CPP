void SelectSort(SqList &L) {  // 算法10.9
  // 对顺序表L作简单选择排序。
  int i,j;
  for (i=1; i<L.length; ++i) { // 选择第i小的记录，并交换到位
    j = SelectMinKey(L, i);  // 在L.r[i..L.length]中选择key最小的记录
    if (i!=j) {                // L.r[i]←→L.r[j];   与第i个记录交换
      RedType temp;
      temp=L.r[i];
      L.r[i]=L.r[j];
      L.r[j]=temp;    
    } 
  }
} // SelectSort
