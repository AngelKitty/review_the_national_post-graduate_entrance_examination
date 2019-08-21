Status SearchBST(BiTree T, KeyType key, BiTree f, BiTree &p) {  
  // 算法9.5(b)
  // 在根指针T所指二叉排序树中递归地查找其关键字等于key的数据元素，
  // 若查找成功，则指针p指向该数据元素结点，并返回TRUE，
  // 否则指针p指向查找路径上访问的最后一个结点并返回FALSE，
  // 指针f指向T的双亲，其初始调用值为NULL
  if (!T) { p = f;  return FALSE; }                     // 查找不成功
  else if (EQ(key, T->data.key)) { p = T;  return TRUE; } // 查找成功
  else if (LT(key, T->data.key)) 
    return SearchBST(T->lchild, key, T, p);  // 在左子树中继续查找
  else  
    return SearchBST(T->rchild, key, T, p);  // 在右子树中继续查找
} // SearchBST
