/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

#include "..\list\list.h"

template <typename T> void crc ( List<T> & L ) { //统计列表的特征（所有元素总和）
   T crc = 0; L.traverse ( Crc<T> ( crc ) ); //以crc为基本操作进行遍历
   printf ( "CRC =" ); print ( crc ); printf ( "\n" ); //输出统计得到的特征
}