void  FindArticul(ALGraph G) {  // 算法7.10
  // 连通图G以邻接表作存储结构，查找并输出G上全部关节点。
  // 全局量count对访问计数。
  int v; 
  struct ArcNode  *p;
  visited[0] = 1;        // 设定邻接表上0号顶点为生成树的根
  for (int i=1;  i<G.vexnum; ++i) visited[i] = 0; // 其余顶点尚未访问
  p = G.vertices[0].firstarc;  
  if(p) {
    v = p->adjvex;
    DFSArticul(G, v);         // 从第v顶点出发深度优先查找关节点。
    if (count < G.vexnum) {   // 生成树的根有至少两棵子树
      printf (0, G.vertices[0].data);  // 根是关节点，输出
      while (p->nextarc) {
        p = p->nextarc;  v = p->adjvex;
        if (visited[v]==0)  DFSArticul(G, v);
      }//while
    }//if
  }//if(p)
} // FindArticul
