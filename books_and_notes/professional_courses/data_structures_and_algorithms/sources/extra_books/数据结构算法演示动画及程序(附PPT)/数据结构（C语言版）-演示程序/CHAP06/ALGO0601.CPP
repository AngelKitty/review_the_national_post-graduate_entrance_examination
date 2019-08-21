Status PreOrderTraverse( BiTree T, Status(*Visit)(ElemType) ) {
   // 算法6.1
   // 采用二叉链表存储结构，Visit是对数据元素操作的应用函数，
   // 先序遍历二叉树T的递归算法，对每个数据元素调用函数Visit。
   // 最简单的Visit函数是：
   //     Status PrintElement( ElemType e ) {  // 输出元素e的值
   //        printf( e );  // 实用时，加上格式串
   //        return OK;
   //     }
   // 调用实例：PreOrderTraverse(T, PrintElement);
   if (T) {
      if (Visit(T->data))
         if (PreOrderTraverse(T->lchild, Visit))
            if (PreOrderTraverse(T->rchild, Visit)) return OK;
      return ERROR;
   } else return OK;
} // PreOrderTraverse
