BiTree SearchBST (BiTree T, KeyType key) {  // 算法9.5(a)
   // 在根指针T所指二叉排序树中递归地查找其关键字等于key的数据元素，
   // 若查找成功，则返回指向该数据元素结点的指针，否则返回空指针
   if (!T || EQ(key, T->data.key)) return T;  // 查找结束
   else if (LT(key, T->data.key)) 
      return SearchBST(T->lchild, key);  // 在左子树中继续查找 
   else 
      return SearchBST(T->rchild, key);  // 在右子树中继续查找
} // SearchBST
