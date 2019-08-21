void BFSTraverse(Graph G, Status (*Visit)(int v )) {//  算法7.6
  // 按广度优先非递归遍历图G。使用辅助队列Q和访问标志数组visited。
  QElemType v,w;
  queue Q;
  QElemType u;
  for (v=0; v<G.vexnum; ++v) visited[v] = FALSE;
  InitQueue(Q);                     // 置空的辅助队列Q
  for (v=0;  v<G.vexnum;  ++v)
    if (!visited[v]) {              // v尚未访问
      visited[v] = TRUE;  Visit(v); // 访问v
      EnQueue(Q, v);                // v入队列
      while (!QueueEmpty(Q)) {
        DeQueue(Q, u);              // 队头元素出队并置为u
        for (w=FirstAdjVex(G, u);  w>=0;  w=NextAdjVex(G, u, w))
          if (!visited[w]) {        // u的尚未访问的邻接顶点w入队列Q
            visited[w] = TRUE;  Visit(w);
            EnQueue(Q, w); 
          }//if   
      }//while                       
    }//if
} // BFSTraverse
