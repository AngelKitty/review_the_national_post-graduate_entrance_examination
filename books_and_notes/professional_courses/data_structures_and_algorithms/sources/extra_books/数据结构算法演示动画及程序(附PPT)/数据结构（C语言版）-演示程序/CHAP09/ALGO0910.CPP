void L_Rotate(BSTree &p) {  // 算法 9.10
  // 对以p↑为根的二叉排序树作左旋处理，处理之后p指向新的树根结点，
  // 即旋转处理之前的右子树的根结点
  BSTree rc;
  rc = p->rchild;            // rc指向*p的右子树根结点
  p->rchild = rc->lchild;    // rc的左子树挂接为*p的右子树
  rc->lchild = p;  p = rc;   // p指向新的根结点
} // L_Rotate
                             