void Insert(BTree &q, int i, KeyType x, BTree ap) {
  // insert the key X between the key[i] and key[i+1] 
  // at the pointer node q
  int n = q->keynum;
  for (int j=n; j>i; j--) {
    q->key[j+1] = q->key[j];
    q->ptr[j+1] = q->ptr[j];
  }
  q->key[i+1] = x;
  q->ptr[i+1] = ap;
  if (ap) ap->parent = q;  
  q->keynum++;
}

void split(BTree &q, int s, BTree &ap) {
  //move key[s+1...m],p->ptr[s...m] int the new pointer *ap
  int i,j,n=q->keynum;
  ap = (BTree)malloc(sizeof(BTNode));
  ap->ptr[0] = q->ptr[s];
  for (i=s+1,j=1; i<=n; i++,j++) {
    ap->key[j] = q->key[i];
    ap->ptr[j] = q->ptr[i];
  }
  ap->keynum = n-s;
  ap->parent = q->parent;
  for (i=0; i<=n-s; i++) 
    //refresh the parent_pointer of the subpointer in new pointer *ap
    if (ap->ptr[i]) ap->ptr[i]->parent = ap;
  q->keynum = s-1;
}

void NewRoot(BTree &T, BTree p, KeyType x, BTree ap) {
  T = (BTree)malloc(sizeof(BTNode));
  T->keynum = 1;  T->ptr[0] = p;  T->ptr[1] = ap;  T->key[1] = x;
  //if (f) ShowBTNode(T);
  if (p) p->parent= T;  
       // refresh the parent_pointer of sub_pointers in *p and *q
  if (ap) ap->parent = T;
  T->parent = NULL;
}

Status InsertBTree(BTree &T, KeyType K, BTree q, int i) { // 算法9.14
  // 在m阶B树T上结点*q的key[i]与key[i+1]之间插入关键字K。
  // 若引起结点过大，则沿双亲链进行必要的结点分裂调整，使T仍是m阶B树。
  BTree ap;
  int finished, needNewRoot, s;
  KeyType x;
  if (!q)                      // T是空树(参数q初值为NULL)
    NewRoot(T, NULL, K, NULL); // 生成仅含关键字K的根结点*T
  else {
    x = K;  ap = NULL;  finished = needNewRoot = FALSE;     
    while (!needNewRoot && !finished) {
      Insert(q, i, x, ap); // 将x和ap分别插入到q->key[i+1]和q->ptr[i+1]
      if (q->keynum < m) finished = TRUE;  // 插入完成
      else {  // 分裂结点*q
        // 将q->key[s+1..m], q->ptr[s..m]和
        // q->recptr[s+1..m]移入新结点*ap
        s = (m+1)/2;   split(q, s, ap);   x = q->key[s];
        if (q->parent) {  // 在双亲结点*q中查找x的插入位置
          q = q->parent;  i = Search(q, x);  
        } else needNewRoot = TRUE;
      } // else
    } // while
    if (needNewRoot)        // 根结点已分裂为结点*q和*ap
      NewRoot(T, q, x, ap); // 生成新根结点*T,q和ap为子树指针
  }
  return OK;
} // InsertBTree
