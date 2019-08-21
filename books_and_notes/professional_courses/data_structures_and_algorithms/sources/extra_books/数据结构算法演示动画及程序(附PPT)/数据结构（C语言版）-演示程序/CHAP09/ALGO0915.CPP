RECORD *SearchDLTree(DLTree T, KeysType K) {  // 算法9.15
   // 在非空双链树T中查找关键字等于K的记录。
   DLTree p;
   int i;
   p = T->first;   i=0;     // 初始化
   while (p && i<K.num) {
      while (p && p->symbol != K.ch[i])  // 查找关键字的第i位
          p = p->next; 
      if (p && i<K.num-1) p = p->first;  // 准备查找下一位
      ++i;
   } // 查找结束
   if (!p) return NULL;     // 查找不成功
   else return p->infoptr;  // 查找成功
} //Search DLTree
