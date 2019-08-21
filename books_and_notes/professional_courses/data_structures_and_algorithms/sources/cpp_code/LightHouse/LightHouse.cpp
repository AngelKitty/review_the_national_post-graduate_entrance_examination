#include <iostream>
#include <cstdio>
#define MAX 1000001
int point_x[MAX];
int point_y[MAX];
int tempL[MAX];
int tempR[MAX];
char buffer[10000];//缓冲区，减少IO操作，缩短读入数据时间
long sum = 0;
using namespace std;
void quickSort(int *array_x,int* array_y,int l,int r)
{
    if(l<r)
    {
        int i=l,j=r;
        int temp = array_x[l];
        int temp1 = array_y[l];
        while(i<j)
        {
            while(i<j && array_x[j]>=temp)
                j--;
            if(i<j)
            {
                array_x[i] = array_x[j];
                array_y[i] = array_y[j];
                ++i;
            }
            while(i<j && array_x[i]<=temp)
                i++;
            if(i<j)
            {
                array_x[j] = array_x[i];
                array_y[j] = array_y[i];
                --j;
            }
        }
        array_x[i] = temp;
        array_y[i] = temp1;
        quickSort(array_x,array_y,l,i-1);
        quickSort(array_x,array_y,i+1,r);
    }

}
void Merge(int* array,int l,int mid,int r)
{
    int i,j,k,n1,n2;
    n1 = mid - l +1;
    n2 = r - mid;
    for(i = 0;i<n1;i++)
    {
        tempL[i] = array[l+i];
    }
    for(i=0;i<n2;i++)
    {
        tempR[i] = array[mid+1+i];
    }
    i = 0;
    j = 0;
    for(k=l;k<r && i<n1 && j<n2;k++)
    {
        if(tempL[i]>tempR[j])//题意各点互异，不会有相等情况
        {
            array[k] = tempR[j++];//x大y小 不符合条件
        }
        else
        {
            array[k] = tempL[i++];
            sum += n2 - j;
        }
    }
    while(i<n1)
    {
        array[k++] = tempL[i++];
    }
    while(j<n2)
    {
        array[k++] = tempR[j++];
    }

}
void MergeSort(int *array,int l,int r)
{
    int mid;
    if(l == r)
        return;
    mid = l+((r-l)>>1);
    MergeSort(array,l,mid);
    MergeSort(array,mid+1,r);
    Merge(array,l,mid,r);
}

int main()
{

    int n;
    scanf("%d",&n);
    setvbuf(stdin,buffer,_IOFBF,10000);
    for(int i=0;i<n;++i)
    {
        scanf("%d %d",&point_x[i],&point_y[i]);
    }
    quickSort(point_x,point_y,0,n-1);
    MergeSort(point_y,0,n-1);
    printf("%ld",sum);
    return 0;
}
