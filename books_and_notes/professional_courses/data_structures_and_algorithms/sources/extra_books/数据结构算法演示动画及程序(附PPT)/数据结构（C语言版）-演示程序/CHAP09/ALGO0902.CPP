int Search_Bin ( SSTable ST, KeyType key ) {  // 算法9.2
   // 在有序表ST中折半查找其关键字等于key的数据元素。
   // 若找到，则函数值为该元素在表中的位置，否则为0。
   int low, high, mid;
   low = 1;  high = ST.length;    // 置区间初值
   while (low <= high) {
      mid = (low + high) / 2;
      if (EQ(key , ST.elem[mid].key)) return mid;    // 找到待查元素
      else if (LT(key, ST.elem[mid].key)) high = mid - 1; 
                             // 继续在前半区间进行查找
      else low = mid + 1;    // 继续在后半区间进行查找
   }
   return 0;                 // 顺序表中不存在待查元素
} // Search_Bin
