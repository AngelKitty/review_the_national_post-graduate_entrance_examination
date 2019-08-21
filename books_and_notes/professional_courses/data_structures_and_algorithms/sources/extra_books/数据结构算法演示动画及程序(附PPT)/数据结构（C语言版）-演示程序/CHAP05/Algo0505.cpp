int GListDepth(GList L) {  // 算法5.5
  // 采用头尾链表存储结构,求广义表L的深度。
  int max, dep;
  GList pp;
  if (!L) return 1;                // 空表深度为1
  if (L->tag == ATOM) return 0;    // 原子深度为0
  for (max=0, pp=L;  pp;  pp=pp->ptr.tp) {
    dep = GListDepth(pp->ptr.hp);  // 求以pp->ptr.hp为头指针的子表深度
    if (dep > max) max = dep;
  }
  return max + 1;        // 非空表的深度是各子表的深度的最大值加1
} // GListDepth
                            

