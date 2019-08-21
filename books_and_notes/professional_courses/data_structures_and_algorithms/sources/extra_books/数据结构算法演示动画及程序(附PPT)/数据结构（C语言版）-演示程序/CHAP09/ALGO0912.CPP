void LeftBalance(BSTree &T) {  // 算法 9.12
  // 对以指针T所指结点为根的二叉树作左平衡旋转处理。
  // 本算法结束时，指针T指向新的根结点
  BSTree lc,rd;
  lc = T->lchild;    // lc指向*T的左子树根结点
  switch (lc->bf) {  // 检查*T的左子树的平衡度，并作相应平衡处理
    case LH:   // 新结点插入在*T的左孩子的左子树上，要作单右旋处理
        T->bf = lc->bf = EH; 
        R_Rotate(T);   
        break;  
    case RH:      // 新结点插入在*T的左孩子的右子树上，要作双旋处理
        rd = lc->rchild;   // rd指向*T的左孩子的右子树根
        switch (rd->bf) {  // 修改*T及其左孩子的平衡因子
          case LH: T->bf = RH;  lc->bf = EH;  break;
          case EH: T->bf = lc->bf = EH;       break;
          case RH: T->bf = EH;  lc->bf = LH;  break;
        } // switch (rd->bf)
        rd->bf = EH;              
        L_Rotate(T->lchild);  // 对*T的左子树作左旋平衡处理
        R_Rotate(T);          // 对*T作右旋平衡处理
  } // switch (lc->bf) 
} // LeftBalance
