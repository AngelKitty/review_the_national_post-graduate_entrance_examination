void ShowBTNode(BTree p) { //display the BTnode
  printf("(");
  for(int i=1; i<=p->keynum; i++)
    printf("%3d", p->key[i]);
  printf(" )");
}

int Search(BTree p, KeyType K) {
   for(int i=0;  i < p->keynum && p->key[i+1] <= K;  i++);
   return i;
}

Result SearchBTree(BTree T, KeyType K, int f) {  // 算法9.13
   // 在m阶B树T上查找关键字K，返回结果(pt,i,tag)
   BTree p, q;
   int found, i, j=0;
   Result R;
   p = T;  q = NULL;  found = FALSE;  i = 0; 
                  // 初始化，p指向待查结点，q指向p的双亲
   while (p && !found) {
      if (f) {
         if (j) printf(" --> ");  
         ShowBTNode(p);           
      }
      i = Search(p, K);  // 在p->key[1..keynum]中查找i，
                         // 使得：p->key[i]<=K<p->key[i+1]
      if (i>0 && p->key[i]==K) found = TRUE;    // 找到待查关键字
      else { q = p;  p = p->ptr[i]; }
      j++;
   }
   if (found) {  // 查找成功
     R.pt = p;  R.i = i;  R.tag = 1;  
   } else {      // 查找不成功
     R.pt = q;  R.i = i;  R.tag = 0; 
   }
   return R;              // 返回结果信息: K的位置(或插入位置)
} // SearchBTree
