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

#include "../_share/util.h"
#include "../UniPrint/print.h"

#if defined(DSA_PQ_LEFTHEAP)
#include "../PQ_LeftHeap/PQ_LeftHeap.h"
#elif defined(DSA_PQ_COMPLHEAP)
#include "../PQ_ComplHeap/PQ_ComplHeap.h"
#elif defined(DSA_PQ_LIST)
#include "../PQ_List/PQ_List.h"
#endif
