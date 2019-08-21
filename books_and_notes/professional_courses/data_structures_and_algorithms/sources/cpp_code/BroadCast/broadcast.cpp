#include <iostream>
#include <cstdio>
#define MAX 1000000

using namespace std;
//使用邻接表存储图
 struct EdgeNode /*边表结点*/
{
    int adjvex; //存储邻接顶点对应下标
    struct EdgeNode *nex;//指向下一个邻接点
};

struct VertexNode //顶点表结点
{
    int castType;//广播状态 1
    bool isVisted;
    int data;//数据
    EdgeNode *firstEdge;//边表头指针
};
struct Graph
{
    VertexNode adjList[MAX];//顶点数组
    int numVertexs;//顶点数
    int numEdges;//边数

}graph;
VertexNode queue[MAX];//模拟队列
int head=0;
int tail =0;
int queuesize=0;
int main()
{
    int n,m;
    scanf("%d%d",&n,&m);
    graph.numVertexs = n;
    graph.numEdges = m;
    if(m<=1)
    {
        printf("1");
        return 0;
    }
    for(int i=0;i<n;i++)//初始化顶点数组
    {
        graph.adjList[i].data = i+1;//顶点从1开始
        graph.adjList[i].isVisted = false;
        graph.adjList[i].firstEdge = NULL;
    }
    int a,b;
    for(int i=0;i<m;i++)//输入信息并构建邻接表
    {
        scanf("%d%d",&a,&b);//距离小于20km的结点（村庄）视为有边连通
        EdgeNode *ea = new EdgeNode;
        EdgeNode *eb = new EdgeNode;
        ea->adjvex = b-1;//下标从0开始
        eb->adjvex = a-1;
        ea->nex = NULL;
        eb->nex = NULL;
        EdgeNode *pa = graph.adjList[a-1].firstEdge;//无向图 一条边更新两个结点
        EdgeNode *pb = graph.adjList[b-1].firstEdge;
        if( pa == NULL)
        {
            graph.adjList[a-1].firstEdge = ea;
        }
        else
        {
            while( pa->nex != NULL)
            {
                pa = pa->nex;
            }
            pa->nex = ea;
        }
        if( pb == NULL)
        {
            graph.adjList[b-1].firstEdge = eb;
        }
        else
        {
            while( pb->nex != NULL)
            {
                pb = pb->nex;
            }
            pb->nex = eb;
        }
    }
    //BFS
    for(int i=0;i<graph.numVertexs;i++)
    {
        if(!graph.adjList[i].isVisted)
        {
            graph.adjList[i].isVisted = true;
            graph.adjList[i].castType = 1;//广播类型
            queue[tail++] = graph.adjList[i];//结点入队
            queuesize++;
            while(queuesize!=0)
            {
                VertexNode node = queue[head++];//取出队列中第一个结点
                queuesize--;
                EdgeNode *pn = node.firstEdge;
                while( pn != NULL)
                {
                    if(!graph.adjList[pn->adjvex].isVisted)//结点未访问过
                    {
                        graph.adjList[pn->adjvex].isVisted = true;
                        graph.adjList[pn->adjvex].castType = -node.castType;//安装不同类型广播
                        queue[tail++] = graph.adjList[pn->adjvex];//入队
                        queuesize++;

                    }
                    else
                    {
                        if(node.castType == graph.adjList[pn->adjvex].castType)
                        {
                            printf("-1");//不可行
                            return 0;
                        }
                    }
                    pn = pn->nex;
                }
            }
        }
    }
    printf("1");//可行
    return 0;
}

