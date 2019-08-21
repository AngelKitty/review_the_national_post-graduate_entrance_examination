int ord(char c) {
   return c-'@';
}

RECORD *SearchTrie(TrieTree T, KeysType K) {  // 算法9.16
   // 在键树T中查找关键字等于K的记录。
   TrieTree p;
   int i;
   for (p=T, i=0;                        // 对K的每个字符逐个查找
      p && p->kind==BRANCH && i<K.num;   // *p为分支结点
      p=p->bh.ptr[ord(K.ch[i])], i++);   // ord求字符在字母表中序号
   if (p && p->kind==LEAF && strcmp(p->lf.K.ch, K.ch)==0)
      return p->lf.infoptr;              // 查找成功
   else return NULL;                     // 查找不成功
} // SearchTrie
