/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

#include "../vector/vector.h"

/******************************************************************************************
 * 生成长度为n的随机有序向量，元素取值来自[min, max]
 * The Art of Computer Programming, Vol.2, Algorithm S
 ******************************************************************************************/
Vector<int> randomSortedVector ( int n, int min, int max );