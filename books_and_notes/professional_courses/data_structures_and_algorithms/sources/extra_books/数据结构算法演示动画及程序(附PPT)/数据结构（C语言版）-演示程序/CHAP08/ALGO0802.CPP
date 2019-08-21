WORD_b* AllocBuddy (FreeList &avail, int n) {  // 算法8.2
  // avail[0..m]为可利用空间表，n为申请分配量，若有不小于n的空闲块，
  // 则分配相应的存储块，并返回其首地址；否则返回NULL
  WORD_b *pa, *pre, *suc, *pi; 
  for (int k=0;
       k<=m && (!avail[k].first || avail[k].nodesize<n+1);
       ++k);                             // 查找满足分配要求的子表
  if (k>m) return NULL;                  // 分配失败，返回NULL
  else {                                 // 进行分配
    pa = avail[k].first;                 // 指向可分配子表的第一个结点
    pre = pa->llink;   suc = pa->rlink;  // 分别指向前驱和后继
    if (pa==suc) avail[k].first = NULL;  // 分配后该子表变成空表
    else {                               // 从子表删去*pa结点
      pre->rlink = suc;   suc->llink = pre;   avail[k].first = suc;
    }
    for (int i=1;  avail[k-i].nodesize>=n+1;  ++i) {
      pi = pa+(int)pow(2, (k-i));  pi->rlink = pi;  pi->llink = pi;
      pi->tag = 0;  pi->kval = k-i;  avail[k-i].first = pi;
    }   // 将剩余块插入相应子表
    pa->tag = 1;  pa->kval = k-(--i);
  }
  return pa;
} // AllocBuddy
