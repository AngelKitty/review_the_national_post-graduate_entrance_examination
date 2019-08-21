#include <iostream>
#include <cstdio>
#include <cstdlib>
#define MAX 1000000
#define GETMAX(x,y) ((x) > (y) ? (x):(y))
using namespace std;

//使用邻接表存储图
 struct EdgeNode /*边表结点*/
{
    int adjvex; //存储邻接顶点对应下标
    int length;//到该结点路径长度
    struct EdgeNode *nex;//指向下一个邻接点
};

struct VertexNode //顶点表结点
{
    int in; //顶点入度
    int data;//数据
    int length;//到该结点路径长度
    EdgeNode *firstEdge;//边表头指针
};
struct Graph
{
    VertexNode adjList[MAX];//顶点数组
    int numVertexs;//顶点数
    int numEdges;//边数

}graph;
int maxLength = 1;
VertexNode topSort[MAX];//保存拓扑排序结果
int main()
{
    int n,m,a,b;
    scanf("%d%d",&n,&m);
    graph.numEdges = m;
    graph.numVertexs = n;
    for(int i=0;i<n;i++)//初始化顶点数组
    {
        graph.adjList[i].data = i+1;
        graph.adjList[i].in = 0;
        graph.adjList[i].length = 1;
        graph.adjList[i].firstEdge = NULL;
    }
    for(int i = 0;i<m;i++)//输入边信息并构建邻接表(拓扑排序) -邻接矩阵（最长路径）
    {
        scanf("%d%d",&a,&b);//输入边的端点
        EdgeNode *e = new EdgeNode;
        e->adjvex = b-1;//数组下标从0开始
        graph.adjList[b-1].in++;//b结点入度加1
        e->nex = NULL;
        EdgeNode *p = graph.adjList[a-1].firstEdge;
        if(p == NULL)
        {
            graph.adjList[a-1].firstEdge = e;
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
    int stack[MAX];//拓扑排序时保存入度为0的结点堆栈
    int stk[MAX];//保存初始入度为0结点
    int top = 0,index,num=0,t=0;
    EdgeNode *e;
    for(int i=0;i<graph.numVertexs;i++)
    {
        if(graph.adjList[i].in == 0)
        {
            stack[top++] = i;//保存结点在数组中的下标
            stk[num++] = i;//入度为0结点数
        }
    }
    while(top!=0)//进行拓扑排序
    {
       index = stack[--top];//出栈，取出入度为零的结点
       for(e = graph.adjList[index].firstEdge;e!=NULL;e=e->nex)//遍历该结点的边表
       {
           int tempindex = e->adjvex;
           //动态规划-更新其后继结点的最大路径长度
           graph.adjList[tempindex].length = GETMAX(graph.adjList[index].length+1,
                                                graph.adjList[tempindex].length);
           maxLength = GETMAX(graph.adjList[tempindex].length,maxLength);
           graph.adjList[tempindex].in--;//将邻接点的入度减1
           if(graph.adjList[tempindex].in == 0)
           {
               stack[top++] = tempindex;//入度为0入栈
           }
       }
    }
    printf("%d",maxLength);
    return 0;
}

