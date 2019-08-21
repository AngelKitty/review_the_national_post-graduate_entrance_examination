void HeapSort(HeapType &H) {  // 算法10.11
   // 对顺序表H进行堆排序。
   int i;
   RedType temp;
   for (i=H.length/2; i>0; --i)  // 把H.r[1..H.length]建成大顶堆
      HeapAdjust ( H, i, H.length );
      for (i=H.length; i>1; --i) {
         temp=H.r[i];
         H.r[i]=H.r[1];
         H.r[1]=temp;  // 将堆顶记录和当前未经排序子序列Hr[1..i]中
                       // 最后一个记录相互交换
         HeapAdjust(H, 1, i-1);  // 将H.r[1..i-1] 重新调整为大顶堆
      }
} // HeapSort
