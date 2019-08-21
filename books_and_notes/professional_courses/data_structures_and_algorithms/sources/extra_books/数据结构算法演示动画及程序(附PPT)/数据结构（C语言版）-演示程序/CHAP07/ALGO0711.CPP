void  DFSArticul(ALGraph G, int v0 ) {  // 算法7.11
  // 从第v0个顶点出发深度优先遍历图G，查找并输出关节点
  int min,w;
  struct ArcNode *p;
  visited[v0] = min = ++count;  // v0是第count个访问的顶点
  for (p=G.vertices[v0].firstarc; p!=NULL; p=p->nextarc) { 
    // 检查v0的每个邻接顶点
    w = p->adjvex;              // w为v0的邻接顶点
    if (visited[w] == 0) {      // w未曾被访问，是v0的孩子
      DFSArticul(G, w);         // 返回前求得low[w]
      if (low[w] < min)  min = low[w];
      if (low[w] >= visited[v0]) 
        printf(v0, G.vertices[v0].data);  // 输出关节点
    } 
    else if (visited[w] < min)  min = visited[w];  
        // w已被访问，w是v0在生成树上的祖先   
  }//for
  low[v0] = min;
} // DFSArticul
