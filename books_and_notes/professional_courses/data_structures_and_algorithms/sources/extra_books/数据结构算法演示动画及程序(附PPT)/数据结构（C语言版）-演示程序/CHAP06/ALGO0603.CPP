Status InOrderTraverse(BiTree T, Status (*Visit)(ElemType)) {  
  // 算法6.3
  // 采用二叉链表存储结构，Visit是对数据元素操作的应用函数。
  // 中序遍历二叉树T的非递归算法，对每个数据元素调用函数Visit。
  stack S;
  BiTree p;
  InitStack(S);  p = T;
  while (p || !StackEmpty(S)) {
    if (p) { Push(S, p);  p = p->lchild; }  // 非空指针进栈，继续左进
    else {       // 上层指针退栈，访问其所指结点，再向右进
      Pop(S, p);  
      if (!Visit(p->data)) return ERROR;
      p = p->rchild;
    }
  }
  return OK;
} // InOrderTraverse
