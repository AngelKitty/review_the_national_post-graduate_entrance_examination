/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once
#pragma warning(disable : 4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../stack/stack.h" //使用栈
#include "priority.h"

__int64 facI ( int n );
float calcu ( float a, char op, float b );
float calcu ( char op, float b );
void displayProgress ( char*, char*, Stack<float>&, Stack<char>&, char* );
void readNumber ( char*&, Stack<float>& );
Operator optr2rank ( char );
char orderBetween ( char, char );
void append ( char*&, float ); //重载
void append ( char*&, char ); //重载
float evaluate ( char* expr, char*& rpn );
