Status CriticalPath(ALGraph G) {  // 算法7.14
  // G为有向网，输出G的各项关键活动。
  Stack T;
  int a,j,k,el,ee,dut;
  char tag;
  ArcNode *p;
  if (!TopologicalOrder(G, T)) return ERROR;
  for(a=0; a<G.vexnum; a++)
    vl[a] = ve[G.vexnum-1];    // 初始化顶点事件的最迟发生时间
  while (!StackEmpty(T))       // 按拓扑逆序求各顶点的vl值
    for (Pop(T, j), p=G.vertices[j].firstarc;  p;  p=p->nextarc) {
      k=p->adjvex;  dut=p->info;     //dut<j,k>
      if (vl[k]-dut < vl[j]) vl[j] = vl[k]-dut;
    }
  for (j=0; j<G.vexnum; ++j)            // 求ee,el和关键活动
    for (p=G.vertices[j].firstarc;  p;  p=p->nextarc) {
      k=p->adjvex;dut=p->info;   
      ee = ve[j];  el = vl[k]-dut;
      tag = (ee==el) ? '*' : ' ';
      printf(j, k, dut, ee, el, tag);   // 输出关键活动
    }
  return OK;
} // CriticalPath
