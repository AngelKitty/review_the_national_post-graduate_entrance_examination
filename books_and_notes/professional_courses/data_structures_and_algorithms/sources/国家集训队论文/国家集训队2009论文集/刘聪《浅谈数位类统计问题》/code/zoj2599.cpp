#include<iostream>
using namespace std;
typedef unsigned long long ULL;
ULL f[20][172];
ULL ten[20];
void init()
{
	f[0][0]=1;
	for (int i=1;i<20;++i)
		for (int j=0;j<=i*9;++j)
			for (int k=0;k<=9;++k) if (j>=k) f[i][j]+=f[i-1][j-k];
	ten[0]=1;
	for (int i=1;i<20;++i) ten[i]=ten[i-1]*10;
}
ULL count(ULL n,int sum)
{
	ULL ans=0;
	for (int i=19;i>=1;--i) {
		sum-=n/ten[i];
		if (sum<0) break;
		n%=ten[i];
		for (int j=0;j<n/ten[i-1];++j) if (sum-j>=0) ans+=f[i-1][sum-j];
	}
	if (sum==n) ++ans;
	return ans;
}
int sumof(ULL n)
{
	int sum=0;
	while (n>0) {
		sum+=n%10;
		n/=10;
	}
	return sum;
}
ULL find(ULL n,int sum,ULL k)
{
	int i;
	ULL k2=k,ans=0;
	for (i=1;i<sum;++i) ans+=count(n,i);
	for (i=0;ten[i+1]<=n;++i);
	for (;k<ten[i];k*=10);
	for (;k>0;k/=10,--i) ans+=count(min(n,k-(k>k2)),sum)-f[i][sum];
	return ans;
}
ULL calc(ULL n,ULL k)
{
	int sum,i;
	ULL ans,k2=k,tot;
	for (sum=1;;++sum) {
		tot=count(n,sum);
		if (k<=tot) break;
		k-=tot;
	}
	for (i=0;ten[i+1]<=n;++i);
	for (ans=1;ans<=9;++ans) {
		int j;
		tot=0;
		for (j=0;j<i;++j) tot+=f[j][sum-ans];
		if (ans<n/ten[i]) tot+=f[i][sum-ans];
		else if (ans==n/ten[i]) tot+=count(n,sum)-count(ans*ten[i]-1,sum);
		if (k<=tot) break;
		k-=tot;
	}
	while (find(n,sumof(ans),ans)!=k2) {
		ans*=10;
		for (i=0;i<=9;++i)
			if (find(n,sum,ans+i)>=k2) break;
		ans+=i;
		if (find(n,sumof(ans),ans)==k2) break;
		--ans;
	}
	return ans;
}
int main()
{
	init();
	ULL n,k;
	while (cin>>n>>k) {
		if (!n) break;
		cout<<find(n,sumof(k),k)<<' '<<calc(n,k)<<endl;
	}
}
