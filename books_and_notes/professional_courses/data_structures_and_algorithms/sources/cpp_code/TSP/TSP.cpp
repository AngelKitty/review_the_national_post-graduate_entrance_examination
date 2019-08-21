#include <iostream>
#include <cstdio>
#include <cstdlib>
#define MAX 100

using namespace std;
//使用邻接表存储图-基于DFS的拓扑排序
enum STATUS
{
    UNVISTED,
    VISTED
};
 struct EdgeNode /*边表结点*/
{
    int adjvex; //存储邻接顶点对应下标
    struct EdgeNode *nex;//指向下一个邻接点
};

struct VertexNode //顶点表结点
{
    int out; //顶点出度
    int data;//数据
  //  STATUS status;//是否被访问过
    EdgeNode *firstEdge;//边表头指针
};
struct Graph //图结构
{
    VertexNode adjList[MAX];//顶点数组
    int numVertexs;//顶点数
    int numEdges;//边数

}graph;

int viste(VertexNode *node);//拓扑排序中从后向前访问结点

int main()
{
    int n,m,a,b;
    scanf("%d%d",&n,&m);
    graph.numVertexs = n;
    graph.numEdges = m;

    for(int i=0;i<n;i++)//初始化顶点数组
    {
        graph.adjList[i].data = i+1;
        graph.adjList[i].out = 0;
     //   graph.adjList[i].status = UNVISTED;//输出拓扑排序时需要
        graph.adjList[i].firstEdge = NULL;
    }
    for(int i = 0;i<m;i++)//输入边信息并构建逆邻接表(拓扑排序) -邻接矩阵（最长路径）
    {
        scanf("%d%d",&a,&b);//输入边的端点
        EdgeNode *e = new EdgeNode;
        e->adjvex = a-1;//数组下标从0开始
        graph.adjList[a-1].out++;//a结点出度加1
        e->nex = NULL;
        EdgeNode *p = graph.adjList[b-1].firstEdge;
        if(p == NULL)
        {
            graph.adjList[b-1].firstEdge = e;
        }
        else
        {
            while (p->nex != NULL)
            {
                p = p->nex;
            }
            p->nex = e;
        }
    }
    int stack[MAX];//保存出度为0的结点堆栈
    int top = 0,index,sum=0,len=0;
    for(int i=0;i<graph.numVertexs;i++)
    {
        if(graph.adjList[i].out == 0)
        {
            stack[top++] = i;//保存结点在数组中的下标
        }
    }
    while(top!=0)//拓扑排序
    {
        index = stack[--top];
        sum = viste(&graph.adjList[index]);
        if(len<sum)
        {
            len = sum;
        }
    }
    printf("%d",len);
    return 0;
}
//深度优先访问结点-清华OJ超时-未用到拓扑排序结果？待优化
int viste(VertexNode *node)
{
    int len = 1,sublen = 0,maxsublen=0;
   // if(node->status == UNVISTED)//输出拓扑排序时需要该判断
  //  {
      //  node->status = VISTED;
        EdgeNode* e;
        for(e=graph.adjList[node->data-1].firstEdge;e!=NULL;e=e->nex)
        {
           sublen = viste(&graph.adjList[e->adjvex]);
           if(maxsublen <sublen)//求出经过结点最多的子结点
           {
               maxsublen = sublen;
           }
        }
        printf("%d ",node->data);
  //  }
    return len+maxsublen;
}

