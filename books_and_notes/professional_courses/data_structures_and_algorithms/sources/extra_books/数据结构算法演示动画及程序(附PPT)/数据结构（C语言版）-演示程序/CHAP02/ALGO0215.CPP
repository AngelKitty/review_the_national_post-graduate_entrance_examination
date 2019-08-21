int Malloc_SL(SLinkList &space) {  // 算法2.15
  // 若备用空间链表非空，则返回分配的结点下标，否则返回0
  int i = space[0].cur;
  if (space[0].cur) space[0].cur = space[space[0].cur].cur;
  return i;
} // Malloc_SL
