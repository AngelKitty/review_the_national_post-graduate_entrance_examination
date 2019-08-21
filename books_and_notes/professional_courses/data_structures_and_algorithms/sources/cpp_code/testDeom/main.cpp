#include<iostream>
#include<math.h>
using namespace std;

int main()
{
    int count,h;
    cin>>h;
    count = 1;

    while(1)
    {
        if((count + count *count) < h)
        {
            count++;
        }
        else
        {
            cout << count - 1;
            return 0;
        }

    }
}
