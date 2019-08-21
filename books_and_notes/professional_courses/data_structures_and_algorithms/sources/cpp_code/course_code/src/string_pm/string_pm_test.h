/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define printString(s) { for (size_t m = strlen(s), k = 0; k < m; k++) printf("%4c", (s)[k]); }

void showProgress ( char*, char*, int, int ); //显示算法进程
void printNext ( int*, int, int ); //打印Next表
int* buildNext ( char* ); //构造Next表
void printBC ( int* ); //打印BC表
int* buildBC ( char* ); //构造BC表
void printGS ( char*, int* ); //打印GS表
int* buildGS ( char* ); //构造GS表