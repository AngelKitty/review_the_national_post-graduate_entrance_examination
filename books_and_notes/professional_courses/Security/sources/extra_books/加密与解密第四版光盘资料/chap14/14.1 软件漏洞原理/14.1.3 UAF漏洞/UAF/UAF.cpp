// UAF.cpp : Defines the entry point for the console application.
//实验环境：
//windows xp sp3 + VC6.0 
//编译选项：Debug（默认配置）


#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

struct A
{
	int par1;
	int par2;
	int par3;
	int par4;
};

struct B
{
	unsigned char q1[8];
	char q2[8];
};

int main(int argc, char* argv[])
{
	struct A *p;
	struct B *q;
	p = (struct A *)malloc(sizeof(struct A));
	free(p);
	q = (struct B *)malloc(sizeof(struct B));
	p->par1 = 1;
	p->par2 = 2;

	return 0;
}

