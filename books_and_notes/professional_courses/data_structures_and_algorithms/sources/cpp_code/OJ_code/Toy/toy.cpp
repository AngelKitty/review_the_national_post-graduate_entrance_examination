#include <iostream>
#include <cstdio>
#include <cstring>

/*
* 思路：将一串数字看做是一个字符串，其全排列总共有8!,每种排列对应一个状态
* 有的状态能够通过题目所给的操作到达，注意题目要求的是从目标状态恢复初始状态
* 所需步数不是从初始状态到达目标状态所需步数，所以要将题目所给的操作逆转过来，
* 才是计算从目标状态返回初始状态的步数。具体做法是利用康拓变换+BFS搜索遍历，
* 提前将所需步数存储在一个输出数组中，数组的秩就是对应状态的康拓变换值，利用
* BFS不断搜索当前状态的下一状态，没有搜索过的状态步长加1，保存到输出数组中，
* 直至搜索完毕。
*/
#define MAX 40320//8! 全部排列方式有8!种

int outMap[MAX]= {0};//输出数组
int factor[8] = {1,1,2,6,24,120,720,5040};//阶乘数组
struct TOY
{
    char str[9];
    int hashVal;//康托变换后的散列值
    TOY()
    {
        hashVal = 0;
    }
    TOY(char *s)
    {
        strcpy(str,s);
        contor();
    }
    void downUpChange(char *s)//下上交换
    {
        for(int i=0;i<8;i++)
        {
            str[7-i] = s[i];
        }
        str[8] = '\0';
        contor();
    }
    void cycleLeftMove(char *s)//循环左移
    {
        for(int i=0;i<4;i++)
        {
            str[i] = s[(i+1)%4];
        }
        for(int i=4;i<8;i++)
        {
            str[i] = s[(i+3)%4+4];
        }
        str[8] = '\0';
        contor();
    }
    void centerMove(char *s)//中心逆时针旋转
    {
        strcpy(str,s);
        str[1] = s[2];
        str[2] = s[5];
        str[5] = s[6];
        str[6] = s[1];
        str[8] = '\0';
        contor();
    }
    void contor()
    {

        hashVal = 0;
        for(int i=0;i<8;i++)
        {
            int temp = 0;
            for(int j=i+1;j<8;j++)
            {
                if(str[j]<str[i])
                {
                    temp++;
                }
            }
            hashVal += temp*factor[7-i];
        }
    }
};
struct Queue
{
    Queue()
    {
        head = 0;
        tail = 0;
        length = 0;
    }
    void push(TOY t)
    {
        _elem[tail++] = t;
        length++;
    }
    TOY pop()
    {
        length--;
        return _elem[head++];

    }
    TOY _elem[MAX*2];//模拟队列
    int head;
    int tail;
    int length;

}queue;


void Init()//初始化，计算所有情况并将步数保存到输出数组中
{
    TOY firstToy("12345678");//初始状态-康托展开hashVal为0
    queue.push(firstToy);//入队
    //BFS
    while(queue.length>0)
    {
        TOY tempHead = queue.pop();//取出第一个节点
        TOY temptail;
        //题目问的是由目标返回初始的步数，因此采取逆操作
        //下上交换
        temptail.downUpChange(tempHead.str);
        if(outMap[temptail.hashVal] == 0 && temptail.hashVal != 0)//未访问过&&不是初始状态
        {
            queue.push(temptail);
            outMap[temptail.hashVal] = outMap[tempHead.hashVal]+1;//步数加1
        }
        //循环左移
        temptail.cycleLeftMove(tempHead.str);
        if(outMap[temptail.hashVal] == 0 && temptail.hashVal != 0)//未访问过&&不是初始状态
        {
            queue.push(temptail);
            outMap[temptail.hashVal] = outMap[tempHead.hashVal]+1;//步数加1
        }
        //中心逆时针旋转
        temptail.centerMove(tempHead.str);
        if(outMap[temptail.hashVal] == 0 && temptail.hashVal != 0)//未访问过&&不是初始状态
        {
            queue.push(temptail);
            outMap[temptail.hashVal] = outMap[tempHead.hashVal]+1;//步数加1
        }

    }

}

int main()
{
    Init();
    int n,num;
    char s[9] ={'\0'};
    scanf("%d",&n);
    while(n--)
    {
        for(int i=0;i<8;i++)
        {
            scanf("%d",&num);
            s[i] = num + '0';
        }
        TOY tempToy(s);
        if(tempToy.hashVal == 0)
        {
            printf("%d\n",0);
        }
        else
        {
            printf("%d\n",outMap[tempToy.hashVal]>0?outMap[tempToy.hashVal]:-1);
        }
    }
    return 0;
}

