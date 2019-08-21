/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

/******************************************************************************************
 * 将Skiplist各方法的实现部分，简洁地引入Skiplist.h
 * 效果等同于将这些实现直接汇入Skiplist.h
 * 在export尚未被编译器支持前，如此可将定义与实现分离，以便课程讲解
 ******************************************************************************************/

#include "Skiplist_get.h"
#include "Skiplist_put.h"
#include "Skiplist_remove.h"
#include "Skiplist_skipSearch.h"
