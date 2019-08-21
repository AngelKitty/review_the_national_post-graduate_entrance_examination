//--- 算法7.4和7.5使用的全局变量 ---
bool visited[MAX_VERTEX_NUM];   // 访问标志数组
Status (* VisitFunc)(int v);    // 函数变量

void DFS(Graph G, int v) {  // 算法7.5
   // 从第v个顶点出发递归地深度优先遍历图G。
   int w;
   visited[v] = true;   VisitFunc(v);  // 访问第v个顶点
   for (w=FirstAdjVex(G, v);  w!=-1;  w=NextAdjVex(G, v, w))
      if (!visited[w])   // 对v的尚未访问的邻接顶点w递归调用DFS
         DFS(G, w);
}
