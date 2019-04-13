//account.h
#ifndef __ACCOUNT_H__
#define __ACCOUNT_H__

class SavingsAccount { //储蓄账户类
private:
	int id;				//账号
	double balance;		//余额
	double rate;		//存款的年利率
	int lastDate;		//上次变更余额的时期
	double accumulation;	//余额按日累加之和
	static double total;	//所有账户的总金额

	//记录一笔帐，date为日期，amount为金额，desc为说明
	void record(int date, double amount);
	//获得到指定日期为止的存款金额按日累积值
	double accumulate(int date) const {
		return accumulation + balance * (date - lastDate);
	}
public:
	//构造函数
	SavingsAccount(int date, int id, double rate);
	int getId() const { return id; }
	double getBalance() const { return balance; }
	double getRate() const { return rate; }
	static double getTotal() { return total; }

	//存入现金
	void deposit(int date, double amount);
	//取出现金
	void withdraw(int date, double amount);
	//结算利息，每年1月1日调用一次该函数
	void settle(int date);
	//显示账户信息
	void show() const;
};

#endif //__ACCOUNT_H__
