#include <iostream>
#include <cstring>
#include <cstdio>
#include <iostream>
#define MAX 100000000
using namespace std;
//链表节点
struct ZumaNode
{
    char data;
    ZumaNode *pred;
    ZumaNode *succ;
    ZumaNode(){}
    ZumaNode(char name):data(name){}
}*header,*trailer;
char buffer[MAX];
char resultBuffer[150000000];//不足够大无法通过最后一个测试用例
int listSize = 0;
//将输入字符串转成列表
void createList(char *str,int len)
{
    header = new ZumaNode();
    header->pred = NULL;
    ZumaNode *rear = header;
  //  rear = header;
    for(int i= 0;i<len;++i)
    {
        ZumaNode *temNode = new ZumaNode(str[i]);
        rear->succ = temNode;
        temNode->pred = rear;
        rear = temNode;
        listSize++;
    }
    trailer = new ZumaNode();
    trailer->pred = rear;
    rear->succ = trailer;
    trailer->succ = NULL;
}

ZumaNode *findNode(int position)
{
    int num = 0;
    ZumaNode *p = header->succ;
    while( num < position && p->succ != NULL)
    {
         p = p->succ;
         num++;
    }
    return p;
}

ZumaNode *delRepet(ZumaNode *currentNode,char data)
{
    ZumaNode *preNode;
    while(1)
    {
       preNode = currentNode->pred;
       int num = 0;
       while(currentNode != trailer && currentNode->data == data)
       {
           currentNode = currentNode->succ;
           num++;
       }
       while(preNode->succ != header && preNode->data == data)
       {
           preNode = preNode->pred;
           num++;
       }
       if(num >= 3)
       {
           listSize -= num;
           ZumaNode *p1,*p2;
           p1 = preNode->succ;
           while(p1 != currentNode)
           {
               p2 = p1->succ;
               delete p1;
               p1 = p2;
           }
           preNode->succ = currentNode;
           currentNode->pred = preNode;
           if(currentNode != trailer)
                data = currentNode->data;//关键点，合并后再次删除
       }
       else
       {
           break;//退出循环
       }
    }
    return preNode;
}

void insertNode(ZumaNode *currentNode,char c)
{
    ZumaNode *temNode = new ZumaNode(c);
    if(currentNode->pred != NULL)
    {
        temNode->succ = currentNode;
        temNode->pred = currentNode->pred;
        currentNode->pred->succ = temNode;
        currentNode->pred = temNode;
    }
    listSize++;
}

int main()
{
    int n,counter,resultLen = 0;
    gets(buffer);
    createList(buffer,strlen(buffer));
    scanf("%d",&n);
    int *arrayposition  = new int[n];
    char *arrayBall = new char[n];
    for(int i=0;i<n;i++)
    {
       scanf("%d %c",&arrayposition[i],&arrayBall[i]);
    }
    counter = 0;
    while(n-->0)
    {
       ZumaNode *temNode = findNode(arrayposition[counter]);
       insertNode(temNode,arrayBall[counter]);
       delRepet(temNode,arrayBall[counter]);
       ZumaNode* result = header;
       if(listSize == 0)
       {
         //  printf("-\n");
           resultBuffer[resultLen++] = '-';
           resultBuffer[resultLen++] = '\n';
       }
       else
       {
         for(int i=0;i<listSize;i++)
         {
           // printf("%c",result->succ->data);
            resultBuffer[resultLen++] = result->succ->data;
            result = result->succ;
         }
         resultBuffer[resultLen++] = '\n';
       //  printf("\n");
       }
       counter++;
    }
    printf("%s",resultBuffer);
    return 0;
}


