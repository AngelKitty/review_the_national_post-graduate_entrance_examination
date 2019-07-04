#include<iostream>
#include<string>
using namespace std;
int f[32][32];
int change(int x,int b)
{
	string s;
	do {
		s=char('0'+x%b)+s;
		x/=b;
	} while (x>0);
	for (int i=0;i<s.size();++i)
		if (s[i]>'1') {
			for (int j=i;j<s.size();++j) s[j]='1';
			break;
		}
	x=0;
	for (int i=0;i<s.size();++i)
		x=x|((s[s.size()-i-1]-'0')<<i);
	return x;
}
void init()//预处理f
{
	f[0][0]=1;
	for (int i=1;i<=31;++i) {
		f[i][0]=f[i-1][0];
		for (int j=1;j<=i;++j) f[i][j]=f[i-1][j]+f[i-1][j-1];
	}
}
int calc(int x,int k)//统计[0..x]内二进制表示含k个1的数的个数
{
	int tot=0,ans=0;//tot记录当前路径上已有的1的数量，ans表示答案
	for (int i=31;i>0;--i)
	{
		if (x&(1<<i)) {
			++tot;
			if (tot>k) break;
			x=x^(1<<i);
		}
		if ((1<<(i-1))<=x) {
			ans+=f[i-1][k-tot];
		}
	}
	if (tot+x==k) ++ans;
	return ans;
}
int main()
{
	int x,y,k,b;
	cin>>x>>y>>k>>b;
	x=change(x,b);y=change(y,b);
	init();
	cout<<calc(y,k)-calc(x-1,k)<<endl;
	return 0;
}
