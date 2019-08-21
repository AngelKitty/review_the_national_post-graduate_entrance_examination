void DFSTraverse(Graph G, Status (*Visit)(int v)) {  // 算法7.4
 // 对图G作深度优先遍历。
 int v;
 VisitFunc = Visit; // 使用全局变量VisitFunc，使DFS不必设函数指针参数
 for (v=0; v<G.vexnum; ++v) visited[v] = false; // 访问标志数组初始化
 for (v=0; v<G.vexnum; ++v) 
   if (!visited[v]) DFS(G, v);         // 对尚未访问的顶点调用DFS
}
