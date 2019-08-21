DuLinkList GetElemP_DuL(DuLinkList va, int i) {  
  // L为带头结点的单链表的头指针。
  // 当第i个元素存在时，其值赋给e并返回OK，否则返回ERROR
  DuLinkList p;
  p = va->next;   
  int j = 1;  // 初始化，p指向第一个结点，j为计数器
  while (p!=va && j<i) { //顺指针向后查找，直到p指向第i个元素或p为空
    p = p->next;
    ++j;
  }
  if (p==va && j<i) return NULL;  // 第i个元素不存在
  else return p;
} // GetElem_L

Status ListInsert_DuL(DuLinkList &L, int i, ElemType e) { //算法2.18
  // 在带头结点的双链循环线性表L的第i个元素之前插入元素e，
  // i的合法值为1≤i≤表长+1。
  DuLinkList p,s;
  if (!(p = GetElemP_DuL(L, i)))  // 在L中确定第i个元素的位置指针p
    return ERROR;                 // p=NULL, 即第i个元素不存在
  if (!(s = (DuLinkList)malloc(sizeof(DuLNode))))
    return ERROR;
  s->data = e;
  s->prior = p->prior;
  p->prior->next = s;
  s->next = p;
  p->prior = s;
  return OK;
} // ListInsert_DuL
