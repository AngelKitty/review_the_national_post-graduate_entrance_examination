/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#include "../UniPrint/print.h"
#include "../vector/vector.h"

Vector<int> //生成长度为n的随机有序向量，元素取值来自[min, max]
randomSortedVector ( int n, int min, int max ) { //ACP, Vol.2, Algorithm S
   /*DSA*/printf ( "creating a random vector of size %d...\n", n );
   Vector<int> A;
   for ( int i = min; i <= max; i++ )
      if ( rand() % ( max - i + 1 ) < n )
         { A.insert ( A.size(), i ); n--; }
   return A;
}