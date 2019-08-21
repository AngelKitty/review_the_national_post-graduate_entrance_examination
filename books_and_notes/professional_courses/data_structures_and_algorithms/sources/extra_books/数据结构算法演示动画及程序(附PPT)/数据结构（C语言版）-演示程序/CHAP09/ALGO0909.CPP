void R_Rotate(BSTree &p) {  //  算法 9.9
  // 对以*p为根的二叉排序树作右旋处理，处理之后p指向新的树根结点，
  // 即旋转处理之前的左子树的根结点
  BSTree lc;
  lc = p->lchild;            // lc指向*p的左子树根结点
  p->lchild = lc->rchild;    // lc的右子树挂接为*p的左子树
  lc->rchild = p;  p = lc;   // p指向新的根结点
} // R_Rotate
