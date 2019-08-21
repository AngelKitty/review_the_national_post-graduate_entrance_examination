#include <iostream>
#include <cstdlib>
#include <cstdio>
using namespace std;
int cmp(const void*a, const void *b)
{
    return *((int *)a) - *((int*)b);
}
int binarySearch(int *array, int e, int lo, int hi, int &flag)//二分查找,左开右闭-返回最接近的
{
    int mid;
    flag = 0;
    while (lo < hi)
    {
        mid = (lo + hi)>>1;
      //  (e<array[mid]) ? (hi = mid ):(flag = 1,lo = mid + 1);//[lo,mid) 或 [mid+1,hi)
        if(e<array[mid])
        {
            hi = mid;
        }
        else
        {
            if(e == array[mid])
                flag = 1;
            lo = mid +1;
        }
    }
    return --lo;//返回最后一个不大于e的下标
}
int main()
{
    int pointNum, cmpNum,count = 0;
    cin >> pointNum >> cmpNum;
    int* array = new int[pointNum];
    int* leftArray = new int[cmpNum];
    int* rightArray = new int[cmpNum];
    for (int i = 0; i < pointNum; i++)
        scanf("%d",&array[i]);
    for (int i = 0; i < cmpNum; i++)
        scanf("%d %d",&leftArray[i],&rightArray[i]);
    qsort(array, pointNum, sizeof(int), cmp);//调用系统快排
    while (cmpNum-- > 0)
    {
        int tem1, tem2,temflag;
        if (leftArray[count] > array[pointNum - 1] || rightArray[count] < array[0])
        {
            printf("0\n");
        }
        else
        {
            tem1 = binarySearch(array, leftArray[count], 0, pointNum,temflag);
            if(temflag == 1)
            {
               tem1--;
            }
            tem2 = binarySearch(array, rightArray[count], 0, pointNum,temflag);
            printf("%d\n",tem2 - tem1);
        }
        count++;
    }
    return 0;
}
