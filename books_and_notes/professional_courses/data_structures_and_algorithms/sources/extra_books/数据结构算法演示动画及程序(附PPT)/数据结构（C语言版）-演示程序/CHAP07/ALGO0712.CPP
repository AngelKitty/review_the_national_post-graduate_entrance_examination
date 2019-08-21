Status TopologicalSort(ALGraph G) {  // 算法7.12
  // 有向图G采用邻接表存储结构。
  // 若G无回路，则输出G的顶点的一个拓扑序列并返回OK，否则ERROR。
  SqStack S;
  int count,k,i;
  ArcNode *p;
  char indegree[MAX_VERTEX_NUM];
  FindInDegree(G, indegree);   // 对各顶点求入度indegree[0..vernum-1]
  InitStack(S);
  for (i=0; i<G.vexnum; ++i)       // 建零入度顶点栈S
    if (!indegree[i]) Push(S, i);  // 入度为0者进栈
  count = 0;                       // 对输出顶点计数
  while (!StackEmpty(S)) {
    Pop(S, i); 
    printf(i, G.vertices[i].data);  ++count;  // 输出i号顶点并计数
    for (p=G.vertices[i].firstarc;  p;  p=p->nextarc) {
      k = p->adjvex;               // 对i号顶点的每个邻接点的入度减1
      if (!(--indegree[k])) Push(S, k);  // 若入度减为0，则入栈
    }
  }
  if (count<G.vexnum) return ERROR;      // 该有向图有回路
  else return OK;
} // TopologicalSort
