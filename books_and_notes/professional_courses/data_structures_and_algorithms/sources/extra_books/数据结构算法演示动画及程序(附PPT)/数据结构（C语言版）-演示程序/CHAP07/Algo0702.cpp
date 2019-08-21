Status CreateUDN(MGraph &G) {//  算法 7.2
  // 采用数组（邻接矩阵）表示法，构造无向网G。
  int i,j,k,w; 
  VertexType  v1,v2; 
  printf("G.vexnum :" );  scanf("%d",&G.vexnum);
  printf("G.arcnum :");   scanf("%d",&G.arcnum);   
  getchar();  /*** 加上此句getchar()!!! ***/
  // scanf("%d,%d,%d",&G.vexnum, &G.arcnum, &IncInfo);      
  for (i=0; i<G.vexnum; i++ ) { 
    printf("G.vexs[%d] : ",i);
    scanf("%c",&G.vexs[i]); 
    getchar();
  } // 构造顶点向量
  for (i=0; i<G.vexnum; ++i )  // 初始化邻接矩阵
    for (j=0; j<G.vexnum; ++j ) {
      G.arcs[i][j].adj = INFINITY; //{adj,info}
      G.arcs[i][j].info= NULL;
    }
  for (k=0; k<G.arcnum; ++k ) {  // 构造邻接矩阵
    printf("v1 (char) : ");  scanf("%c", &v1);getchar();
    printf("v2 (char) : ");  scanf("%c", &v2);getchar();
    printf("w (int) : " );   scanf("%d", &w); getchar();   
                             // 输入一条边依附的顶点及权值
    i = LocateVex(G, v1);  j = LocateVex(G, v2);    
          // 确定v1和v2在G中位置
    G.arcs[i][j].adj = w;                // 弧<v1,v2>的权值
    // if (IncInfo) scanf(G.arcs[i][j].info); // 输入弧含有相关信息
    G.arcs[j][i].adj = G.arcs[i][j].adj; // 置<v1,v2>的对称弧<v2,v1>
  }
  return OK;
} // CreateUDN
