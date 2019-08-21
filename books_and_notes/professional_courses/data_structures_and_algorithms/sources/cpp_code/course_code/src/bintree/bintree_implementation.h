/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

#include <iostream>
using namespace std;

/******************************************************************************************
 * 将binTree各方法的实现部分，简洁地引入binTree.h
 * 效果等同于将这些实现直接汇入binTree.h
 * 在export尚未被编译器支持前，如此可将定义与实现分离，以便课程讲解
 ******************************************************************************************/
#include "BinTree_updateHeight.h"
#include "BinTree_insert.h"
#include "BinTree_attach.h"
#include "BinTree_remove.h"
#include "BinTree_secede.h"
