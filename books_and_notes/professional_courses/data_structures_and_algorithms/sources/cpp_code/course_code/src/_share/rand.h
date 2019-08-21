/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

#include <stdlib.h>
#include <time.h> //通常用当前时刻设置随机种子

/******************************************************************************************
 * 在[0, range)内随机生成一个数
 ******************************************************************************************/
static int dice ( int range ) { return rand() % range; } //取[0, range)中的随机整数
static int dice ( int lo, int hi ) { return lo + rand() % ( hi - lo ); } //取[lo, hi)中的随机整数
static float dice ( float range ) { return rand() % ( 1000 * ( int ) range ) / ( float ) 1000.; }
static double dice ( double range ) { return rand() % ( 1000 * ( int ) range ) / ( double ) 1000.; }
static char dice ( char range ) { return ( char ) ( 32 + rand() % 96 ); }