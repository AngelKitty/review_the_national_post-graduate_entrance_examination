void MergeList_Sq(SqList La, SqList Lb, SqList &Lc) {  // 算法2.7
  // 已知顺序线性表La和Lb的元素按值非递减排列。
  // 归并La和Lb得到新的顺序线性表Lc，Lc的元素也按值非递减排列。
  ElemType *pa,*pb,*pc,*pa_last,*pb_last;
  pa = La.elem;  pb = Lb.elem;
  Lc.listsize = Lc.length = La.length+Lb.length;
  pc = Lc.elem = (ElemType *)malloc(Lc.listsize*sizeof(ElemType));
  if (!Lc.elem)
    exit(OVERFLOW);   // 存储分配失败
  pa_last = La.elem+La.length-1;
  pb_last = Lb.elem+Lb.length-1;
  while (pa <= pa_last && pb <= pb_last) {  // 归并
    if (*pa <= *pb) *pc++ = *pa++;
    else *pc++ = *pb++;
  }
  while (pa <= pa_last) *pc++ = *pa++;      // 插入La的剩余元素
  while (pb <= pb_last) *pc++ = *pb++;      // 插入Lb的剩余元素
} // MergeList
