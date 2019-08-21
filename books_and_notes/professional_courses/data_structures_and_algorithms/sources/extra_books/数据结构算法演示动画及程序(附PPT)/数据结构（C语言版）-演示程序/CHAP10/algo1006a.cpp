int Partition(SqList &L, int low, int high) {  // 算法10.6(a)
   // 交换顺序表L中子序列L.r[low..high]的记录，使枢轴记录到位，
   // 并返回其所在位置，此时，在它之前（后）的记录均不大（小）于它
   KeyType pivotkey;
   RedType temp;
   pivotkey = L.r[low].key;     // 用子表的第一个记录作枢轴记录
   while (low<high) {           // 从表的两端交替地向中间扫描
      while (low<high && L.r[high].key>=pivotkey) --high;
      temp=L.r[low];
      L.r[low]=L.r[high];
      L.r[high]=temp;           // 将比枢轴记录小的记录交换到低端
      while (low<high && L.r[low].key<=pivotkey) ++low;
      temp=L.r[low];
      L.r[low]=L.r[high];
      L.r[high]=temp;           // 将比枢轴记录大的记录交换到高端
   }
   return low;                  // 返回枢轴所在位置
} // Partition
