Status merge_mfset(MFSet &S, int i, int j) {  // 算法6.9
   // S.nodes[i]和S.nodes[j]分别为S中两个互不相交的子集Si和Sj的根结点。
   // 求并集Si∪Sj。
   if (i<0 || i>S.n || j<0 || j>S.n) return ERROR;
   S.nodes[i].parent = j;
   return OK;
} // merge_mfset
