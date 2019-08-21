Status TopologicalOrder(ALGraph G, Stack &T) {  // 算法7.13
  // 有向网G采用邻接表存储结构，求各顶点事件的最早发生时间ve(全局变量)。
  // T为拓扑序列定点栈，S为零入度顶点栈。
  // 若G无回路，则用栈T返回G的一个拓扑序列，且函数值为OK，否则为ERROR。
  Stack S;int count=0,k;
  char indegree[40];
  ArcNode *p;
  InitStack(S);
  FindInDegree(G, indegree);  // 对各顶点求入度indegree[0..vernum-1]
  for (int j=0; j<G.vexnum; ++j)     // 建零入度顶点栈S
    if (indegree[j]==0) Push(S, j);  // 入度为0者进栈
  InitStack(T);//建拓扑序列顶点栈T
  count = 0;  
  for(int i=0; i<G.vexnum; i++) ve[i] = 0;  // 初始化
  while (!StackEmpty(S)) {
    Pop(S, j);  Push(T, j);  ++count;       // j号顶点入T栈并计数
    for (p=G.vertices[j].firstarc;  p;  p=p->nextarc) {
      k = p->adjvex;            // 对j号顶点的每个邻接点的入度减1
      if (--indegree[k] == 0) Push(S, k);   // 若入度减为0，则入栈
      if (ve[j]+p->info > ve[k])  ve[k] = ve[j]+p->info;
    }//for  *(p->info)=dut(<j,k>)
  }//while
  if (count<G.vexnum) return ERROR;  // 该有向网有回路
  else return OK;
} // TopologicalOrder
