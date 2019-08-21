int Search_Seq(SSTable ST, KeyType key) {  // 算法9.1
   // 在顺序表ST中顺序查找其关键字等于key的数据元素。
   // 若找到，则函数值为该元素在表中的位置，否则为0。
   int i=0;
   ST.elem[0].key=key;   // "哨兵"
   for (i=ST.length;  ST.elem[i].key!=key;  --i);  // 从后往前找
   return i;      // 找不到时，i为0
} // Search_Seq
