#include<iostream>
#include<cstring>
using namespace std;
const int MXK=101;
const int D=10000;
class bignum {
public:
	bignum();
	bignum(int);
	friend bignum operator+(const bignum&,const bignum&);
	friend bignum operator-(const bignum&,const bignum&);
	friend bignum operator*(const bignum&,int);
	friend bool operator<=(const bignum&,const bignum&);
	friend bignum div2(const bignum&);
	void print();
private:
	int s[10],l;
};
bignum::bignum()
{
	memset(s,0,sizeof(s));
	l=1;
}
bignum::bignum(int a)
{
	memset(s,0,sizeof(s));
	l=1;
	s[0]=a;
	while (s[l-1]>=D) {
		s[l]=s[l-1]/D;
		s[l-1]%=D;
		++l;
	}
}
bignum operator+(const bignum& a,const bignum& b)
{
	bignum c;
	c.l=a.l<b.l?b.l:a.l;
	int i,x=0;
	for (i=0;i<c.l;++i) {
		x=x/D+a.s[i]+b.s[i];
		c.s[i]=x%D;
	}
	while (x>=D) {
		x/=D;
		c.s[c.l++]=x%D;
	}
	return c;
}
bignum operator-(const bignum& a,const bignum& b)
{
	bignum c=a;
	int i;
	for (i=0;i<c.l;++i) {
		c.s[i]-=b.s[i];
		if (c.s[i]<0) {
			c.s[i]+=D;
			c.s[i+1]--;
		}
	}
	while (c.l>1 && c.s[c.l-1]==0) --c.l;
	return c;
}
bignum operator*(const bignum& a,int b)
{
	bignum c;
	c.l=a.l;
	int i,x=0;
	for (i=0;i<c.l;++i) {
		x=x/D+a.s[i]*b;
		c.s[i]=x%D;
	}
	while (x>=D) {
		x/=D;
		c.s[c.l++]=x%D;
	}
	return c;
}
bool operator<=(const bignum& a,const bignum& b)
{
	if (a.l!=b.l) return a.l<b.l;
	int i;
	for (i=a.l-1;i>=0;--i) if (a.s[i]!=b.s[i]) return a.s[i]<b.s[i];
	return true;
}
bignum div2(const bignum& a)
{
	int i;
	bignum b=a;
	for (i=b.l-1;i>=0;--i) {
		if (b.s[i]&1) {
			b.s[i]--;
			if (i>0) b.s[i-1]+=D;
		}
		b.s[i]/=2;
	}
	while (b.l>1 && b.s[b.l-1]==0) --b.l;
	return b;
}
void bignum::print()
{
	int i;
	printf("%d",s[l-1]);
	for (i=l-2;i>=0;--i) printf("%04d",s[i]);
	printf("\n");
}
int k,m;
bignum f[MXK],p[MXK];
bignum count(bignum n)//统计1-n中所有数有多少个1
{
	int i,cur=0;
	bignum ans=0;
	for (i=k;i>0;--i) {
		if (p[i]<=n) {
			n=n-p[i];
			++cur;
		}
		if (p[i-1]<=n) {
			ans=ans+f[i-1]+p[i-1]*cur;
		}
	}
	ans=ans+cur;
	if (n<=1 && 1<=n) ans=ans+1;
	return ans;
}
bignum calc(bignum s)//统计1-?中的1的个数不超过s的最大?
{
	bignum a=0;
	int i,cur=0;
	for (i=k;i>0;--i) {
		if (f[i-1]+p[i-1]*cur<=s) {
			a=a+p[i-1];
			s=s-(f[i-1]+p[i-1]*cur);
			++cur;
		}
	}
	if (cur<=s) a=a+1;
	return a-1;
}
bool check(bignum s)
{
	bignum a=1;
	int cnt=0;
	while (a+1<=p[k]) {
		bignum b=calc(count(a-1)+s);
		if (++cnt>m) return false;
		a=b+1;
	}
	return true;
}
int main()
{
	int i;
	cin>>k>>m;
	p[0]=1;
	for (i=1;i<=k;++i) p[i]=p[i-1]+p[i-1];
	f[0]=0;
	for (i=1;i<=k;++i) f[i]=f[i-1]+f[i-1]+p[i-1];
	bignum l=0,r=count(p[k]-1);
	while (l+2<=r) {
		bignum mid=div2(l+r);
		if (check(mid)) r=mid;else l=mid;
	}
	r.print();
}
