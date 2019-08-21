/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
using namespace std;

/******************************************************************************************
 * 测试、演示用小工具
 ******************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <typeinfo.h>
#include "comparator.h"

#include "checkOrder_Elem.h"
#include "checkOrder_list.h"
#include "checkOrder_vector.h"

#include "double_Elem.h"
#include "increase_Elem.h"
#include "hailstone_Elem.h"
#include "increase_list.h"
#include "increase_vector.h"

#include "crc_Elem.h"
#include "crc_list.h"
#include "crc_vector.h"

#include "rand.h" //随机数

#if defined(DSA_DEBUG) //编译开关，控制调试输出
#define DSA(x)  { x } //输出
#else
#define DSA(x) //不输出
#endif