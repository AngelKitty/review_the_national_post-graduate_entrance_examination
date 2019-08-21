/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

/******************************************************************************************
 * 将Quadlist各方法的实现部分，简洁地引入Quadlist.h
 * 效果等同于将这些实现直接汇入Quadlist.h
 * 在export尚未被编译器支持前，如此可将定义与实现分离，以便课程讲解
 ******************************************************************************************/

#include "Quadlist_initialize.h"

#include "Quadlist_insert.h"
#include "Quadlist_remove.h"

//#include "Quadlist_search.h"

#include "Quadlist_traverse.h"
