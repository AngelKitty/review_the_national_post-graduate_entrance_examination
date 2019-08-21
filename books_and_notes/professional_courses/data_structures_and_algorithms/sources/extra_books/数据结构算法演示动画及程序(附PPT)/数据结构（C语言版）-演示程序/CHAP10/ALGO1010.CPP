void HeapAdjust(HeapType &H, int s, int m) {  // 算法10.10
  // 已知H.r[s..m]中记录的关键字除H.r[s].key之外均满足堆的定义，
  // 本函数调整H.r[s]的关键字，使H.r[s..m]成为一个大顶堆
  // （对其中记录的关键字而言）
  int j;
  RedType rc;
  rc = H.r[s];
  for (j=2*s; j<=m; j*=2) {   // 沿key较大的孩子结点向下筛选
    if (j<m && H.r[j].key<H.r[j+1].key) ++j; // j为key较大的记录的下标
    if (rc.key >= H.r[j].key) break;         // rc应插入在位置s上
    H.r[s] = H.r[j];  s = j;
  }
  H.r[s] = rc;  // 插入
} // HeapAdjust
