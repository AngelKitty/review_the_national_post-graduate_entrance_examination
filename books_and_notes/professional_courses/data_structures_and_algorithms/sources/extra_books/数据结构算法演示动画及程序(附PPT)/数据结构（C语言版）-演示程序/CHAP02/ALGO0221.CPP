Status MergeList_L(NLinkList &La, NLinkList &Lb, NLinkList &Lc, 
                 int (*compare)(ElemType, ElemType)) {  // 算法2.21
  // 已知单链线性表La和Lb的元素按值非递减排列。
  // 归并La和Lb得到新的单链线性表Lc，Lc的元素也按值非递减排列。
  NLink ha, hb;
  Position pa, pb, q;
  ElemType a, b;
  if (!InitList(Lc)) return ERROR;  // 存储空间分配失败
  ha = GetHead(La);      // ha和hb分别指向La和Lb的头结点
  hb = GetHead(Lb);
  pa = NextPos(La, ha);  // pa和pb分别指向La和Lb中当前结点
  pb = NextPos(Lb, hb);
  while (pa && pb) {     // La和Lb均非空
    a = GetCurElem(pa);  // a和b为两表中当前比较元素
    b = GetCurElem(pb);
    if ((*compare)(a, b)<=0) {  // a≤b
      DelFirst(ha, q);  Append(Lc, q);  pa = NextPos(La, pa);  
    } else {   // a＞b
      DelFirst(hb, q);  Append(Lc, q);  pb = NextPos(Lb, pb);  
    }
  } // while
  if (pa) Append(Lc, pa);        // 链接La中剩余结点
  else Append(Lc, pb);           // 链接Lb中剩余结点
  FreeNode(ha);   FreeNode(hb);  // 释放La和Lb的头结点
  return OK;
} // MergeList_L
                               


