Status GetElem_L(LinkList &L,int i, ElemType &e) {  // 算法2.8
  // L为带头结点的单链表的头指针。
  // 当第i个元素存在时，其值赋给e并返回OK，否则返回ERROR
  LinkList p;
  p = L->next;   
  int j = 1;           // 初始化，p指向第一个结点，j为计数器
  while (p && j<i) {   // 顺指针向后查找，直到p指向第i个元素或p为空
    p = p->next;  ++j;
  }
  if ( !p || j>i ) return ERROR;  // 第i个元素不存在
  e = p->data;   // 取第i个元素
  return OK;
} // GetElem_L
