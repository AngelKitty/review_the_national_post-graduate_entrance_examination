Status InsertBST(BiTree &T, ElemType e) {  // 算法9.6
  // 当二叉排序树T中不存在关键字等于e.key的数据元素时，
  // 插入e并返回TRUE，否则返回FALSE
  BiTree p,s;
  if (!SearchBST(T, e.key, NULL, p)) {   // 查找不成功
    s = (BiTree)malloc(sizeof(BiTNode));
    s->data = e;  s->lchild = s->rchild = NULL;  
    if (!p) T = s;        // 插入 s 为新的根结点
    else if (LT(e.key, p->data.key)) p->lchild=s; // 插入s为左孩子
    else p->rchild = s;   // 插入 s 为右孩子
    return TRUE;
  } else return FALSE;  // 树中已有关键字相同的结点，不再插入
} // Insert BST
