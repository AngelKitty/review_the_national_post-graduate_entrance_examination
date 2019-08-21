Status ListInsert_L(NLinkList L, int i, ElemType e) {  // 算法2.20
  // 在带头结点的单链线性表L的第i个元素之前插入元素e
  NLink h,s;
  if (!LocatePos(L, i-1, h))  
    return ERROR;  // i值不合法
  if (!MakeNode(s, e))
    return ERROR;  // 结点存储分配失败
  InsFirst(h, s);  // 对于从第i结点开始的链表，第i-1结点是它的头结点
  return OK;
}  // ListInsert_L
