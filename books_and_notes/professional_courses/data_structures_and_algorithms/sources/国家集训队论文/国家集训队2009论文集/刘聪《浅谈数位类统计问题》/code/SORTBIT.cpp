#include<iostream>
#include<string>
using namespace std;
int f[32][32];
void init()
{
	f[0][0]=1;
	for (int i=1;i<=31;++i) {
		f[i][0]=f[i-1][0];
		for (int j=1;j<=i;++j) f[i][j]=f[i-1][j]+f[i-1][j-1];
	}
}
int calc(int x,int k)
{
	if (x<0) return 0;
	int tot=0,ans=0;
	for (int i=31;i>0;--i)
	{
		if (x&(1U<<i)) {
			++tot;
			if (tot>k) break;
			x=x^(1U<<i);
		}
		if ((1<<(i-1))<=x) {
			ans+=f[i-1][k-tot];
		}
	}
	if (tot+x==k) ++ans;
	return ans;
}
int find(int x,int y,int k)
{
	int i;
	for (i=0;;++i) {
		int t=calc(y,i)-calc(x-1,i);
		if (k<=t) break;
		k-=t;
	}
	if (calc(x,i)-calc(x-1,i)==k) return x;
	unsigned l=x,r=y,tmp=calc(x-1,i);
	while (l+1<r) {
		int mid=(l+r)/2;
		int t=calc(mid,i)-tmp;
		if (t<k) l=mid;else r=mid;
	}
	return l+1;
}
int main()
{
	init();
	int T;
	scanf("%d",&T);
	while (T--) {
		int x,y,k;
		bool negative=false;
		scanf("%d%d%d",&x,&y,&k);
		if (y==0) {
			if (k==1) {
				cout<<0<<endl;
				continue;
			}
			--y,--k;
		}
		if (x<0) x^=1<<31,y^=1<<31,negative=true;
		int ans=find(x,y,k);
		if (negative) ans^=1<<31;
		cout<<ans<<endl;
	}
}
