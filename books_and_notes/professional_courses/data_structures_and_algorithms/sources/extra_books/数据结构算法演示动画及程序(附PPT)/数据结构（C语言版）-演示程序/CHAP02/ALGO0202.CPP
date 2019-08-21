void MergeList(List La, List Lb, List &Lc) {  // 算法2.2
  // 已知线性表La和Lb中的元素按值非递减排列。
  // 归并La和Lb得到新的线性表Lc，Lc的元素也按值非递减排列。
  int La_len, Lb_len;
  ElemType ai, bj;	  
  int i=1, j=1, k=0;
  InitList(Lc);
  La_len = ListLength(La);    
  Lb_len = ListLength(Lb);
  while ((i <= La_len) && (j <= Lb_len)) {  // La和Lb均非空
    GetElem(La, i, ai);
    GetElem(Lb, j, bj);
    if (ai <= bj) {
      ListInsert(Lc, ++k, ai);
      ++i;
    } else { 
      ListInsert(Lc, ++k, bj);
      ++j;
    }
  }
  while (i <= La_len) {
    GetElem(La, i++, ai);  ListInsert(Lc, ++k, ai);
  }
  while (j <= Lb_len) {
    GetElem(Lb, j++, bj);  ListInsert(Lc, ++k, bj);
  }
} // MergeList
