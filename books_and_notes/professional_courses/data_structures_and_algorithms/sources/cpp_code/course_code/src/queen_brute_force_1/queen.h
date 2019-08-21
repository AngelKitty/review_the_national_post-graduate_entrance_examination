/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

#include<iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../stack/stack.h"

#define QUEEN_MAX 20

typedef enum {Continuous, Step} RunMode;

extern RunMode runMode; //运行模式
extern int nSolu; //解的总数
extern int nCheck; //尝试的总次数

void place5Queens_BruteForce();
void displaySolution ( int* solu, int n );
bool collide ( int* solu, int k );

void placeQueens ( int n, int k );

void place2Queens();
void place3Queens();
void place4Queens();
void place5Queens();