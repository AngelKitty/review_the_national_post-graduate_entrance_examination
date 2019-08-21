void QSort(SqList &L, int low, int high) {  //算法10.7
  // 对顺序表L中的子序列L.r[low..high]进行快速排序
  int pivotloc;
  if (low < high) {                      // 长度大于1
    pivotloc = Partition(L, low, high);  // 将L.r[low..high]一分为二
    QSort(L, low, pivotloc-1); // 对低子表递归排序，pivotloc是枢轴位置
    QSort(L, pivotloc+1, high);          // 对高子表递归排序
  }
} // QSort
