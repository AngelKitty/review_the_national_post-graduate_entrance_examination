const	int e = 16; // 不保留<=e的剩余量

Space AllocBoundTag (Space &pav, int n) {  //  算法8.1
  // 若有不小于n的空闲块，则分配相应的存储块，并返回其首地址；
  // 否则返回NULL。
  // 若分配后可利用空间表不空，则pav指向表中刚分配过的结点的后继结点
  Space p;
  for (p=pav;
       p && p->size<n && p->rlink!=pav;
       p=p->rlink);                   // 查找不小于n的空闲块
  if (!p || p->size<n) return NULL;   // 找不到，返回空指针
  else {                              // p指向找到的空闲块
    Space f = FootLoc(p);             // 指向底部
    pav = p->rlink;                   // pav指向*p结点的后继结点。
    if (p->size-n <= e) {             // 整块分配，不保留<=e的剩余量
      if (pav==p) pav = NULL;         // 可利用空间表变为空表
      else {                          // 在表中删除分配的结点
        pav->llink = p->llink;  p->llink->rlink = pav;
      }
      p->tag = f->tag = 1;            // 修改分配结点的头部和底部标志
    } else {                          // 分配该块的后n个字
      f->tag = 1;                     // 修改分配块的底部标志
      p->size -= n;                   // 置剩余块大小
      f = FootLoc(p);                 // 指向剩余块底部
      f->tag = 0;   f->uplink = p;    // 设置剩余块底部
      p = f+1;                        // 指向分配块头部
      p->tag = 1;   p->size = n;      // 设置分配块头部
    }
    return p;                         // 返回分配块首地址
  }
} // AllocBoundTag
