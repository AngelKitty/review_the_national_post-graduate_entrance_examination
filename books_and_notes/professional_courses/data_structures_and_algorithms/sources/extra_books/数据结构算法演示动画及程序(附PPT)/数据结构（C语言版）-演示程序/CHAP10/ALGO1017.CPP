void RadixSort(SLList &L) {  // 算法10.17
   // L是采用静态链表表示的顺序表。
   // 对L作基数排序，使得L成为按关键字自小到大的有序静态链表，
   // L.r[0]为头结点。
   int i;
   ArrType f, e;
   for (i=1; i<L.recnum; ++i) L.r[i-1].next = i;
   L.r[L.recnum].next = 0;     // 将L改造为静态链表
   for (i=0; i<L.keynum; ++i) {  
      // 按最低位优先依次对各关键字进行分配和收集
      Distribute(L, i, f, e);    // 第i趟分配
      Collect(L, i, f, e);       // 第i趟收集
      print_SLList2(L, i);
   }
} // RadixSort
