Status InsertAVL(BSTree &T, ElemType e, Boolean &taller) { // 算法9.11
  // 若在平衡的二叉排序树T中不存在和e有相同关键字的结点，
  // 则插入一个数据元素为e的新结点，并返回1，否则返回0。
  // 若因插入而使二叉排序树失去平衡，则作平衡旋转处理，
  // 布尔变量taller反映T长高与否
  if (!T) {  // 插入新结点，树"长高"，置taller为TRUE
    T = (BSTree) malloc (sizeof(BSTNode));  T->data = e;
    T->lchild = T->rchild = NULL;  T->bf = EH;  taller = TRUE;
  }
  else {
    if (EQ(e.key, T->data.key))    // 树中已存在和e有相同关键字的结点
      { taller = FALSE;  return 0; } // 则不再插入
    if (LT(e.key, T->data.key)) {    // 应继续在*T的左子树中进行搜索
      if (InsertAVL(T->lchild, e, taller)==0) return 0;    // 未插入
      if (taller)  // 已插入到*T的左子树中且左子树"长高"
        switch (T->bf) {   // 检查*T的平衡度
          case LH:   // 原本左子树比右子树高，需要作左平衡处理
             LeftBalance(T);   taller = FALSE;  break;
          case EH:   // 原本左、右子树等高，现因左子树增高而使树增高
             T->bf = LH;  taller = TRUE;  break;
          case RH:   // 原本右子树比左子树高，现左、右子树等高
             T->bf = EH;  taller = FALSE;  break;   
        } // switch (T->bf)
    } // if
    else {    // 应继续在T↑的右子树中进行搜索
      if (InsertAVL(T->rchild, e, taller)==0) return 0;
      if (taller)         // 已插入到*T的右子树且右子树长高
        switch (T->bf) {  // 检查*T的平衡度
          case LH:   // 原本左子树比右子树高，现左、右子树等高
             T->bf = EH;  taller = FALSE;  break;   
          case EH:   // 原本左、右子树等高，现因右子树增高而使树增高
             T->bf = RH;  taller = TRUE;  break;
          case RH:   // 原本右子树比左子树高，需要作右平衡处理
             RightBalance(T);  taller = FALSE;  break;
        } // switch (T->bf)
    } // else
  } // else 
  return 1;
} //InsertAVL
