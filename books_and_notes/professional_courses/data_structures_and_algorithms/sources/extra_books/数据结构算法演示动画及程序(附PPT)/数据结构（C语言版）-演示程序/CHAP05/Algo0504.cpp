Status CreateSMatrix_OL (CrossList &M) { // 算法5.4
  // 创建稀疏矩阵M。采用十字链表存储表示。
  // if (M) free(M);
  // scanf(&m, &n, &t );    // 输入M的行数、列数和非零元个数
  OLNode *p,*q;
  int i,j,e;
  int m=random(4,6), n=random(4,6), t=random(4,5);
  M.mu=m;  M.nu=n;  M.tu=t; 
  if (!(M.rhead = (OLink *)malloc((m+1)*sizeof(OLink)))) return ERROR;
  if (!(M.chead = (OLink *)malloc((n+1)*sizeof(OLink)))) return ERROR;
  for(int a=1;a<=m;a++)  // 初始化行列头指针向量；各行列链表为空链表
     M.rhead[a]=NULL;
  for(int b=1;b<=n;b++) M.chead[b]=NULL;
  for ( int c=1; c<=t; c++) { // 按任意次序输入非零元
    scanf(&i,&j,&e);
    if (!(p = (OLNode *)malloc(sizeof(OLNode)))) return ERROR;
    p->i=i;  p->j=j;  p->e=e;  p->down=NULL;  p->right=NULL; // 新结点
    if (M.rhead[i] == NULL || M.rhead[i]->j > j) {
      p->right = M.rhead[i];   M.rhead[i]= p;  
    } else {  // 寻查在行表中的插入位置
      for (q=M.rhead[i]; (q->right) && (q->right->j<j);  q=q->right);
      p->right = q->right;  q ->right = p;     
    }  // 完成行插入
    if (M.chead[j] == NULL || M.chead[j]->i > i) {
      p->down = M.chead[j];   M.chead[j]= p;  
    } else {  // 寻查在列表中的插入位置
      for ( q=M.chead[j]; (q->down) && q->down->i <i;  q = q->down );
        p->down = q->down;  q->down = p;    
    }  // 完成列插入
  } // for
  return OK;
} // CreateSMatrix_OL
                            

