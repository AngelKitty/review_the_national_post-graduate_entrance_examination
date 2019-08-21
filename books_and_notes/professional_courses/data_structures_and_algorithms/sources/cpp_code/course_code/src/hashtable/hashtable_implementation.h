/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

/******************************************************************************************
 * 将Hashtable各方法的实现部分，简洁地引入Hashtable.h
 * 效果等同于将这些实现直接汇入Hashtable.h
 * 在export尚未被编译器支持前，如此可将定义与实现分离，以便课程讲解
 ******************************************************************************************/
//#include "../Comparator/Comparator.h"
#include "../_share/release.h"
#include "../_share/util.h"

#include "Hashtable_constructor.h"
#include "Hashtable_destructor.h"
#include "Hashtable_hashCode.h"
#include "Hashtable_probe4Hit.h"
#include "Hashtable_probe4Free.h"
#include "Hashtable_get.h"
#include "Hashtable_put.h"
#include "Hashtable_remove.h"
#include "Hashtable_rehash.h"
