Status Delete(BiTree &p) {  // 算法9.8
  // 从二叉排序树中删除结点p，并重接它的左或右子树
  BiTree q, s;
  if (!p->rchild) {  // 右子树空则只需重接它的左子树
    q = p;  p = p->lchild;  free(q);
  } else if (!p->lchild) {  // 只需重接它的右子树
    q = p;  p = p->rchild;  free(q);
  } else {  // 左右子树均不空
    q = p;  s = p->lchild;
    while (s->rchild)   // 转左，然后向右到尽头
      { q = s;  s = s->rchild; }
    p->data = s->data;                  // s指向被删结点的“后继”
    if (q != p) q->rchild = s->lchild;  // 重接*q的右子树
    else q->lchild = s->lchild;         // 重接*q的左子树
    free(s);    
  }
  return TRUE;
} // Delete
