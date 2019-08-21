Status InOrderThreading(BiThrTree &Thrt, BiThrTree T) {  // 算法6.6
   // 中序遍历二叉树T，并将其中序线索化，Thrt指向头结点。
   if (!(Thrt = (BiThrTree)malloc(sizeof(BiThrNode)))) exit(OVERFLOW);
   Thrt->LTag = Link;  Thrt->RTag =Thread;  // 建头结点
   Thrt->rchild = Thrt;              // 右指针回指
   if (!T) Thrt->lchild = Thrt;      // 若二叉树空，则左指针回指
   else {
      Thrt->lchild = T;    pre = Thrt;
      InThreading(T);  // 算法6.7：中序遍历进行中序线索化
      pre->rchild = Thrt;  pre->RTag = Thread; // 最后一个结点线索化
      Thrt->rchild = pre;  
   }
   return OK;
} // InOrderThreading
