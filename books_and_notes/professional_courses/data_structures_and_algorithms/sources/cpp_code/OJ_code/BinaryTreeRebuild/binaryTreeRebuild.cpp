#include <iostream>
#include <cstdio>

#define MAX 10000001
using namespace std;


int preOder[MAX];
int postOder[MAX];
char buffer[MAX];//缓冲区，减少IO操作，缩短读入数据时间
int findPos(int source[],int len,int key)
{
    int count;
    for(count = 0;count < len;count++)
    {
        if(source[count] == key)
        {
            return count;
        }
    }
}

void rebuild(int preTree[],int postTree[],int len)
{
    int limit;
    if(len == 1)
    {
        printf("%d ",preTree[0]);
        return;
    }
    limit = findPos(postTree,len,preTree[1]);//找到左右子树分界点
    rebuild(preTree+1,postTree,limit+1);
    printf("%d ",preTree[0]);
    rebuild(preTree+limit+1+1,postTree+limit+1,len - limit -1-1);

}

int main()
{
    int n;
    scanf("%d",&n);
    setvbuf(stdin,buffer,_IOFBF,MAX);
    for(int i=0;i<n;i++)
    {
        scanf("%d",&preOder[i]);
    }
    for(int i=0;i<n;i++)
    {
        scanf("%d",&postOder[i]);
    }
    rebuild(preOder,postOder,n);
    return 0;
}

