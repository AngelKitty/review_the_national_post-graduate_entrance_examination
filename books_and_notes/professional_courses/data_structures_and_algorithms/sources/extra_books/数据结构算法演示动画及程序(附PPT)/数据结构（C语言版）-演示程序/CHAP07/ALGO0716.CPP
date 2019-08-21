void ShortestPath_FLOYD(MGraph G, PathMatrix P[], DistancMatrix &D) {
  // 算法7.16
  // 用Floyd算法求有向网G中各对顶点v和w之间的最短路径P[v][w]及其
  // 带权长度D[v][w]。若P[v][w][u]为TRUE，则u是从v到w当前求得最
  // 短路径上的顶点。
  int v,w,u,i;
  for (v=0; v<G.vexnum; ++v)        // 各对结点之间初始已知路径及距离
    for (w=0; w<G.vexnum; ++w) {
      D[v][w] = G.arcs[v][w].adj;
      for (u=0; u<G.vexnum; ++u) P[v][w][u] = FALSE;
      if (D[v][w] < INFINITY) {     // 从v到w有直接路径
        P[v][w][v] = P[v][w][w] = TRUE;
      }//if
    }//for
  for (u=0; u<G.vexnum; ++u)
    for (v=0; v<G.vexnum; ++v)
      for (w=0; w<G.vexnum; ++w)
        if (D[v][u]+D[u][w] < D[v][w]) {  // 从v经u到w的一条路径更短
          D[v][w] = D[v][u]+D[u][w];
          for (i=0; i<G.vexnum; ++i)
            P[v][w][i] =(P[v][u][i] || P[u][w][i]);
        }//if
} // ShortestPath_FLOYD
