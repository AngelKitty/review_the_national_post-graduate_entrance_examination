Status mix_mfset(MFSet &S, int i, int j) {  // 算法6.10
   // S.nodes[i]和S.nodes[j]分别为S中两个互不相交的子集Si和Sj的根结点
   // 求并集Si∪Sj。
   if (i<0 || i>S.n || j<0 || j>S.n)  return ERROR;
   if (S.nodes[i].parent>S.nodes[j].parent) {   // Si所含成员数比Sj少
      S.nodes[j].parent+=S.nodes[i].parent;
      S.nodes[i].parent=j;
   } else {                                     // Sj的元素比Si少
      S.nodes[i].parent+=S.nodes[j].parent;
      S.nodes[j].parent=i;
   }
   return OK;
} // mix_mfset
