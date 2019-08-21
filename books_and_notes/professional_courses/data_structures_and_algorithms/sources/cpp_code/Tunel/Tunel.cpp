#include <iostream>
#include <cstdio>
#define MAX 20000000
using namespace std;
char resultBuff[10000000];
char buffer[1000000];//缓冲区，减少IO操作，缩短读入数据时间
int resultLen = 0;
class Queue
{
public:
    Queue(int capacity)
    {
        this->capacity = capacity;
        data = new int[capacity];
        currentSize = 0;
        front = 0;
        rear = 0;
    }
    void enQueue(int e)
    {
        data[rear] = e;
        rear = (rear +1) % capacity;
        currentSize++;
    }
    int deQueue()
    {
        int val = data[front];
        front = (front+1) % capacity;
        currentSize--;
        printf("%d\n",val);
        return val;
    }
    bool empty()
    {
        return currentSize == 0;
    }
    bool full()
    {
        return currentSize == capacity;
    }
private:
    int front;
    int rear;
    int currentSize;
    int capacity;
    int *data;
};
struct dataNode
{
    int val;
    int counter;
};
class DQueue
{
public:
    DQueue(int capacity)
    {
        this->capacity = capacity;
        data = new dataNode[capacity];
        front = 0;
        rear = 0;
        currentSize = 0;
    }
    int removeFront()
    {
        int val = data[front].val;
        front = (front+1)%capacity;
        currentSize--;
        return val;
    }

    dataNode removeRear()
    {

        if(rear == 0)
        {
            rear = capacity -1;
        }
        else
        {
            rear--;
        }
        dataNode temNode = data[rear];
        currentSize--;
        return temNode;
    }
    void insertRear(int e)
    {
        dataNode temNode;
        temNode.val = e;
        data[rear] = temNode;
        rear = (rear + 1)%capacity;
        currentSize++;
    }
    dataNode& getRear()
    {
        int pos;
        if(rear == 0)
        {
            pos = capacity - 1;
        }
        else
        {
            pos = rear -1;
        }
        return data[pos];
    }
    dataNode& getFront()
    {
        return data[front];
    }

    bool empty()
    {
        return currentSize == 0;
    }
    bool full()
    {
        return currentSize == capacity;
    }

private:
    int front;
    int rear;
    int currentSize;
    int capacity;
    dataNode *data;

};
int main()
{
    int n,x;
    char buf[16];
    Queue queue(MAX);
    DQueue dqueue(MAX);
    scanf("%d",&n);
    setvbuf(stdin,buffer,_IOFBF,1000000);
    while (n-->0)
    {
        scanf("%s",buf);
        if(buf[0] == 'E')
        {
            scanf("%d",&x);
            if(!queue.full())
            {
                queue.enQueue(x);
                int a = 1;
                while(!dqueue.empty() && dqueue.getRear().val <= x)
                {
                    a += dqueue.removeRear().counter;
                }
                dqueue.insertRear(x);
                dqueue.getRear().counter = a;
            }

        }
        else
        {
            if(buf[0] == 'D')
            {
                if(!queue.empty())
                {
                    queue.deQueue();
                    if(!(--dqueue.getFront().counter))
                    {
                        dqueue.removeFront();
                    }
                }
            }
            else
            {
                int val = dqueue.getFront().val;
                printf("%d\n",val);
            }
        }
    }
    return 0;
}

