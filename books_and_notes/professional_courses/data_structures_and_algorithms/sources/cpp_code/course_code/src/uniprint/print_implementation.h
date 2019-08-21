/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

/******************************************************************************************
 * 将Print各方法的实现部分，简洁地引入Print.h
 * 效果等同于将这些实现直接汇入Print.h
 * 在export尚未被编译器支持前，如此可将定义与实现分离，以便课程讲解
 ******************************************************************************************/

#include "Print_BinNode.h"
#include "Print_BinTree.h"
#include "Print_BTree.h"
#include "Print_PQ_List.h"
#include "Print_PQ_ComplHeap.h"
#include "Print_PQ_LeftHeap.h"
#include "Print_Entry.h"
#include "Print_Quadlist.h"
#include "Print_Skiplist.h"
#include "Print_Hashtable.h"
#include "print_GraphMatrix.h"

#include "Print_traversable.h"