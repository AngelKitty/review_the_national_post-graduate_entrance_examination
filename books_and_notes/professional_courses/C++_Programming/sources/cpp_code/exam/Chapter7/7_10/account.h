//account.h
#ifndef __ACCOUNT_H__
#define __ACCOUNT_H__
#include "date.h"
#include "accumulator.h"
#include <string>

class Account { //账户类
private:
	std::string id;	//帐号
	double balance;	//余额
	static double total; //所有账户的总金额
protected:
	//供派生类调用的构造函数，id为账户
	Account(const Date &date, const std::string &id);
	//记录一笔帐，date为日期，amount为金额，desc为说明
	void record(const Date &date, double amount, const std::string &desc);
	//报告错误信息
	void error(const std::string &msg) const;
public:
	const std::string &getId() const { return id; }
	double getBalance() const { return balance; }
	static double getTotal() { return total; }
	//显示账户信息
	void show() const;
};

class SavingsAccount : public Account { //储蓄账户类
private:
	Accumulator acc;	//辅助计算利息的累加器
	double rate;		//存款的年利率
public:
	//构造函数
	SavingsAccount(const Date &date, const std::string &id, double rate);
	double getRate() const { return rate; }
	//存入现金
	void deposit(const Date &date, double amount, const std::string &desc);
	//取出现金
	void withdraw(const Date &date, double amount, const std::string &desc);
	//结算利息，每年1月1日调用一次该函数
	void settle(const Date &date);
};

class CreditAccount : public Account { //信用账户类
private:
	Accumulator acc;	//辅助计算利息的累加器
	double credit;		//信用额度
	double rate;		//欠款的日利率
	double fee;			//信用卡年费

	double getDebt() const {	//获得欠款额
		double balance = getBalance();
		return (balance < 0 ? balance : 0);
	}
public:
	//构造函数
	CreditAccount(const Date &date, const std::string &id, double credit, double rate, double fee);
	double getCredit() const { return credit; }
	double getRate() const { return rate; }
	double getFee() const { return fee; }
	double getAvailableCredit() const {	//获得可用信用
		if (getBalance() < 0) 
			return credit + getBalance();
		else
			return credit;
	}
	//存入现金
	void deposit(const Date &date, double amount, const std::string &desc);
	//取出现金
	void withdraw(const Date &date, double amount, const std::string &desc);
	//结算利息和年费，每月1日调用一次该函数
	void settle(const Date &date);

	void show() const;
};

#endif //__ACCOUNT_H__
