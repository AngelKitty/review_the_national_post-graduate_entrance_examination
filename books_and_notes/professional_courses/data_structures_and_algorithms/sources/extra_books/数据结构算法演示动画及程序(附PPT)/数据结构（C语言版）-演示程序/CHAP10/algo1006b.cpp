int Partition(SqList &L, int low, int high) {  // 算法10.6(b)
   // 交换顺序表L中子序列L.r[low..high]的记录，使枢轴记录到位，
   // 并返回其所在位置，此时，在它之前（后）的记录均不大（小）于它
   KeyType pivotkey;
   L.r[0] = L.r[low];            // 用子表的第一个记录作枢轴记录
   pivotkey = L.r[low].key;      // 枢轴记录关键字
   while (low<high) {            // 从表的两端交替地向中间扫描
      while (low<high && L.r[high].key>=pivotkey) --high;
      L.r[low] = L.r[high];      // 将比枢轴记录小的记录移到低端
      while (low<high && L.r[low].key<=pivotkey) ++low;
      L.r[high] = L.r[low];      // 将比枢轴记录大的记录移到高端
   }
   L.r[low] = L.r[0];            // 枢轴记录到位
   return low;                   // 返回枢轴位置
} // Partition
