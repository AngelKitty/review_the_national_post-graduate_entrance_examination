/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

#pragma warning(disable : 4996 4800)
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "../_share/release.h"

class Bitmap { //位图Bitmap类：以空间作为补偿，节省初始化时间（既允许插入，亦支持删除）
private:
   Rank* F; Rank N; //规模为N的向量F，记录[k]被标记的次序（即其在栈T[]中的秩）
   Rank* T; Rank top; //容量为N的栈T，记录被标记各位秩的栈，以及栈顶指针

protected:
   inline bool valid ( Rank r ) { return ( 0 <= r ) && ( r < top ); }
   inline bool erased ( Rank k ) //判断[k]是否曾被标记过，且后来又被清除
   { return valid ( F[k] ) && ! ( T[ F[k] ] + 1 + k ); } //这里约定：T[ F[k] ] = - 1 - k

public:
   Bitmap ( Rank n = 8 ) //按指定（或默认）规模创建比特图（为测试暂时选用较小的默认值）
   { N = n; F = new Rank[N]; T = new Rank[N]; top = 0; } //在O(1)时间内隐式地初始化
   ~Bitmap() { delete [] F; delete [] T; } //析构时释放空间

// 接口
   inline void set ( Rank k ) { //插入
      if ( test ( k ) ) return; //忽略已带标记的位
      if ( !erased ( k ) ) F[k] = top++; //若系初次标记，则创建新校验环
      T[ F[k] ] = k; //若系曾经标记后被清除，则恢复原校验环
   }
   inline void clear ( Rank k ) //删除
   {  if ( test ( k ) ) T[ F[k] ] = - 1 - k;  } //忽略不带标记的位
   inline bool test ( Rank k ) //测试
   {  return valid ( F[k] ) && ( k == T[ F[k] ] );  }
};
