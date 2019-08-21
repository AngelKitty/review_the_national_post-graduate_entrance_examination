Status CreateDG(OLGraph &G) {  // 算法7.3
  // 采用十字链表存储表示，构造有向图G（G.kind=DG）。
  //scanf(&G.vexnum, &G.arcnum, &IncInfo);  
  int i,j,k;
  char v1,v2;
  int IncInfo=0;
  struct ArcBox *p;
  scanfInit();  // 输入初始化
  scanf(&G.vexnum, &G.arcnum, &IncInfo);  // 自定义输入函数
  for (i=0; i<G.vexnum; ++i) {            // 构造表头向量
    scanf(&G.xlist[i].data);              // 输入顶点值
    G.xlist[i].firstin = G.xlist[i].firstout = NULL;  // 初始化指针
  }
  for (k=0; k<G.arcnum; ++k) {  // 输入各弧并构造十字链表
    scanf(&v1, &v2);            // 输入一条弧的始点和终点
    i=LocateVex(G, v1);  j=LocateVex(G, v2); // 确定v1和v2在G中位置
		
    p=(ArcBox *) malloc (sizeof (ArcBox));     // 假定有足够空间
       // *p = {i, j, G.xlist[j].firstin, G.xlist[i].firstout, NULL} 
       // {tailvex, headvex, hlink, tlink, info}
    p->tailvex=i;
    p->headvex=j;
    p->hlink=G.xlist[j].firstin;
    p->tlink=G.xlist[j].firstout;
    G.xlist[j].firstin = G.xlist[i].firstout = p; 
        // 完成在入弧和出弧链头的插入
    //if (IncInfo) Input(*p->info); // 输入弧含有相关信息，此略!!!
  }
  return OK;
} // CreateDG
