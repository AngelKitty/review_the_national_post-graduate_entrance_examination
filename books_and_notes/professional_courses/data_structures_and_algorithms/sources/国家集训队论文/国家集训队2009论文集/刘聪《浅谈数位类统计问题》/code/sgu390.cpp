#include<iostream>
#include<cstring>
using namespace std;
const double MXR=1000000000000000000LL;
struct ret {
	long long a;
	int b;
	ret operator+=(ret y)//合并操作，为了方便进行了重载
	{
		a+=y.a;
		b=y.b;
		return *this;
	}
};
long long ten[19]={1};
ret f[19][170][1000];
bool vis[19][170][1000];
int k;
ret calc(int h,int sum,int rem)//对一棵高度为h，所有元素的数位和需要额外+sum，在此树之前的小组有rem的空间的完全十叉树进行记忆化搜索
{
	if (vis[h][sum][rem]) return f[h][sum][rem];
	vis[h][sum][rem]=true;
	ret &ans=f[h][sum][rem];
	if (h==0) {//h=0的情况
		if (rem==0) {
			ans.a=1;
			if (sum<k) ans.b=k-sum;
			else ans.b=0;
		} else {
			ans.a=0;
			if (rem>sum) ans.b=rem-sum;
			else ans.b=0;
		}
		return ans;
	}
	ans.a=0;ans.b=rem;
	int i;
	for (i=0;i<=9;++i) {//h>0时，根据十个儿子合并得出
		ans+=calc(h-1,sum+i,ans.b);
	}
	return ans;
}
ret count(long long l,long long r)
{
	long long l2=l,r2=r;
	int i,j,h=-1,suml=0,sumr=0;
	for (i=18;i>=0;--i) {//求出其LCA、两个数分别的数位和
		if (h<0 && l/ten[i]!=r/ten[i]) h=i;
		suml+=l/ten[i];
		sumr+=r/ten[i];
		l%=ten[i];r%=ten[i];
	}
	ret ans;
	if (h<0) {//l=r的情况
		ans.a=1;
		if (suml>=k) ans.b=0;else ans.b=k-suml;
		return ans;
	}
	ans.a=0;ans.b=0;
	l=l2,r=r2;
	ans+=calc(0,suml,ans.b);//不要忘记l
	for (i=0;i<h;++i) {//向上走
		suml-=l%ten[i+1]/ten[i];
		for (j=l%ten[i+1]/ten[i]+1;j<=9;++j) ans+=calc(i,suml+j,ans.b);
	}
	suml-=l%ten[h+1]/ten[h];
	for (j=l%ten[h+1]/ten[h]+1;j<r%ten[h+1]/ten[h];++j) ans+=calc(h,suml+j,ans.b);//在LCA的下一层，向右走
	for (i=h-1;i>=0;--i) {//向下走
		suml+=r%ten[i+2]/ten[i+1];
		for (j=0;j<r%ten[i+1]/ten[i];++j) ans+=calc(i,suml+j,ans.b);
	}
	ans+=calc(0,suml+r%10,ans.b);//不要忘记r
	return ans;
}
int main()
{
	long long l,r;
	cin>>l>>r>>k;
	bool flag=false;
	for (int i=1;i<=18;++i) ten[i]=10*ten[i-1];
	if (r==MXR && l<r) {
		--r;
		flag=true;
	}
	ret ans=count(l,r);
	if (flag) {
		if (ans.b==0) {
			++ans.a;
			ans.b=k;
		}
		--ans.b;
	}
	if (ans.b>0) --ans.a;
	cout<<ans.a<<endl;
}
