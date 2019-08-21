void DFSTree(Graph G, int v, CSTree &T) {  // 算法7.8
  // 从第v个顶点出发深度优先遍历图G，建立以T为根的生成树
  int w;
  CSTree p,q;
  bool first =TRUE;
  visited[v] = TRUE;   
  for (w=FirstAdjVex(G, v);  w!=-1;  w=NextAdjVex(G, v, w))
    if (!visited[w]) {
      p = (CSTree) malloc (sizeof (CSNode)); // 分配孩子结点
      p->data = GetVex(G,w);
      p->firstchild=NULL;
      p->nextsibling=NULL;
      if (first) {    // w是v的第一个未被访问的邻接顶点
        T->firstchild = p;  first = FALSE;   // 是根的左孩子结点
      } else {        // w是v的其它未被访问的邻接顶点
        q->nextsibling = p;    // 是上一邻接顶点的右兄弟结点
      }
      q = p;
      DFSTree(G,w,q) ;
    }//if
} // DFSTree
