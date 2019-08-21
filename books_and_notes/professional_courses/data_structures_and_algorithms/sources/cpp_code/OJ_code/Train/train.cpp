#include <iostream>
#include <cstdio>
#include <cstring>

#define MAX 20000000
//#define DEBUG
using namespace std;
int outArray[MAX];
char resultBuffer[50000000];
int resultLen = 0;
class MyStack
{
public:
    MyStack(int capacity)
    {
       this->capacity = capacity;
       currentSize = 0;
       st = new int[capacity];
    }
    bool push(int e)
    {
       if(currentSize<capacity)
       {
          strcpy(resultBuffer+resultLen,"push\n");
          resultLen += 5;
          st[currentSize++] = e;
          return true;
       }
       else
       {
           return false;
       }
    }
    int top()
    {
        return st[currentSize-1];
    }

    int pop()
    {
        strcpy(resultBuffer+resultLen,"pop\n");
        resultLen += 4;
        currentSize--;
        return st[currentSize-1];
    }
    bool empty()
    {
        return (currentSize == 0);
    }
    bool full()
    {
        return (currentSize == capacity);
    }

private:
    int *st;
    int currentSize;
    int capacity;
};
int main()
{
    int m,n,i,j,r;
    scanf("%d%d",&n,&m);
    if(m == 0)
    {
        printf("No\n");
        return 0;
    }
    for(i=0;i<n;i++)
    {
      scanf("%d",&outArray[i]);
    }
    MyStack stack(m);
    i=0,j=0,r=1;
    while(i<n)
    {
        if(outArray[i] == r)
        {
            if(stack.full())
            {
                printf("No\n");
                return 0;
            }
#ifdef DEBUG
            cout<<"push "<<r<<endl<<"pop "<<r<<endl;
#endif
            strcpy(resultBuffer+resultLen,"push\npop\n");
            resultLen += 9;
            i++;
            r++;
            continue;
        }
        if(outArray[i]<r)
        {
            if(outArray[i] == stack.top())
            {
#ifdef DEBUG
                cout<<"pop "<<outArray[i]<<endl;
#endif
                stack.pop();
                i++;
                continue;
            }
            else
            {
                printf("No\n");
                return 0;
            }
        }
        if(outArray[i] > r)
        {
            while(outArray[i] > r)
            {
                if(stack.full())
                {
                    printf("No\n");
                    return 0;
                }
#ifdef DEBUG
                cout<<"push "<<r<<endl;
#endif
                stack.push(r++);
            }
            if(stack.full())
            {
                printf("No\n");
                return 0;
            }
#ifdef DEBUG
                cout<<"push "<<r<<endl<<"pop "<<r<<endl;
#endif

            strcpy(resultBuffer+resultLen,"push\npop\n");
            resultLen += 9;
            r++;
            i++;
        }
    }
    printf("%s",resultBuffer);
    return 0;
}

