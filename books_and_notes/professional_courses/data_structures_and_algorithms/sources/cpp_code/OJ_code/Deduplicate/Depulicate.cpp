#include <iostream>
#include <cstring>
#include <cstdio>
#include <cmath>
#define MAX 400003
char buffer[1000000];//缓冲区，减少IO操作，缩短读入数据时间
using namespace std;
struct HashNode
{
  HashNode()
  {
    memset(str,'\0',41);
    isEmpty = true;
    isDepulicate = false;
  }
  char str[41];//保存字符串
  bool isEmpty;//是否为空
  bool isDepulicate;//是否已有重复
};
struct HashTable
{
    HashNode bullet[MAX];//存储结点的桶数组
    unsigned int hash(char *str, int len);
    bool put(char *str,int len);//添加元素
    char *depulist[MAX];
    int depulen;//重复字符串数
};

unsigned int HashTable::hash(char *str,int len)//散列函数
{
    unsigned int keyValue = 0;
   /* for(int i=0;i<len;i++)//hash函数版本1
    {
        keyValue += str[i]*((int)pow(33,len-i));//将字符出现顺序也考虑进去
    }
    keyValue = (keyValue*3 + 7)%MAX;*/
    unsigned int seed = 131;
    unsigned int hash = 0;
    for(int i=0;i<len;i++)//版本2 BKDhash
    {
        hash = hash*seed+str[i];
    }
    keyValue = hash&0x7fffffff;
    return keyValue;
}
bool HashTable::put(char *str, int len)
{
    int index = hash(str,len)%MAX;
    int pre = 1,step = 1;
    while(!bullet[index].isEmpty)
    {
        if(strcmp(bullet[index].str,str) == 0)//相等 重复字符串
        {
            if(!bullet[index].isDepulicate)//多次重复的字符串最后只输出一次
            {
                bullet[index].isDepulicate = true;//已经保存了该重复字符串
                depulist[depulen] = new char[41];
                strcpy(depulist[depulen++],str);
            }
            return false;
        }
        else
        {
            index = (index + pre*step*step)%MAX;
            while(index<0)
            {
                index += MAX;
            }
            pre = -pre;
            step++;
        }
    }
    //存入空桶
    strcpy(bullet[index].str,str);
    bullet[index].isEmpty = false;
    return true;

}

int main()
{
    int n;
    HashTable *hashtable = new HashTable;
    char str[41];
    hashtable->depulen = 0;
    scanf("%d",&n);
    setvbuf(stdin,buffer,_IOFBF,1000000);
    for(int i=0;i<n;i++)
    {
        scanf("%s",str);
        hashtable->put(str,strlen(str));
    }
    if(hashtable->depulen > 0)
    {
        for(int i=0;i<hashtable->depulen;i++)
        {
            printf("%s\n",hashtable->depulist[i]);
        }
    }
    return 0;
}




