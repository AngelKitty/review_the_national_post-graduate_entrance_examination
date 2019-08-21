void DFSForest(Graph G, CSTree &T) {  // 算法7.7
  // 建立无向图G的深度优先生成森林的(最左)孩子(右)兄弟链表T
  int v; int j=0;
  CSTree p,q;
  T = NULL;
  for (v=0; v<G.vexnum; ++v)  visited[v] = FALSE;
  for (v=0; v<G.vexnum; ++v)
    if (!visited[v]) {   // 第v顶点为新的生成树的根结点
      p= (CSTree)malloc(sizeof(CSNode));  // 分配根结点
      p->data=GetVex(G,v);                // 给该结点赋值
      p->firstchild=NULL;
      p->nextsibling=NULL;
      if (!T) T = p;           // 是第一棵生成树的根(T的根)
      else q->nextsibling = p; // 其它生成树的根(前一棵的根的“兄弟”)
      q = p;                   // q指示当前生成树的根
      DFSTree(G, v,p);         // 建立以p为根的生成树
    }//if
} // DFSForest
