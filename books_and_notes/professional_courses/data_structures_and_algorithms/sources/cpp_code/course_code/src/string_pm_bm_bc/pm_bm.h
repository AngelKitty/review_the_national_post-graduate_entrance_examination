/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

//////////////////////////////////////////////////////////////////////////
// Boyer-Moore算法
//////////////////////////////////////////////////////////////////////////
void     ShowProgress ( String, String,  int,  int );
#define  CARD_CHAR_SET     256   //Cardinality of charactor set
int*     BuildBC ( String ); //构造Bad Charactor Shift表
int*     suffixes ( String );
int*     BuildGS ( String ); //构造Good Suffix Shift表
