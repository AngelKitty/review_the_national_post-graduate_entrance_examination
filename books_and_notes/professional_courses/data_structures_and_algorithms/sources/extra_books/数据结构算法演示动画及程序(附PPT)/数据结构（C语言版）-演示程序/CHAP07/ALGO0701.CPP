Status CreateGraph( MGraph &G ) {  // 算法7.1
    // 采用数组(邻接矩阵)表示法，构造图G。
    scanf(&G.kind);  // 自定义输入函数，读入一个随机值
    switch (G.kind) {
        case  DG: return CreateDG(G);   // 构造有向图G
        case  DN: return CreateDN(G);   // 构造有向网G
        case UDG: return CreateUDG(G);  // 构造无向图G
        case UDN: return CreateUDN(G);  // 构造无向网G，算法7.2
        default : return ERROR;
    }
} // CreateGraph
