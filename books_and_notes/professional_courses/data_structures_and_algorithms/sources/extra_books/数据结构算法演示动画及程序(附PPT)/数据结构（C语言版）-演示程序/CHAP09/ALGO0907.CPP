Status DeleteBST(BiTree &T,  KeyType key) { // 算法9.7 
  // 若二叉排序树T中存在关键字等于key的数据元素时，
  // 则删除该数据元素结点p，并返回TRUE；否则返回FALSE
  if (!T) return FALSE;       // 不存在关键字等于key的数据元素
  else {
    if (EQ(key, T->data.key))  // 找到关键字等于key的数据元素
      return Delete(T); 
    else if (LT(key, T->data.key)) return DeleteBST(T->lchild, key);
    else return DeleteBST(T->rchild, key);
  }
} // DeleteBST
