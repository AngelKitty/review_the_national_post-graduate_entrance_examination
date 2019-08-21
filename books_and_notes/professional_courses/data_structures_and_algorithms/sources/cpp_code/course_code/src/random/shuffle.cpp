/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include <iostream>
/*DSA*/using namespace std;

void shuffle ( int A[], int n ) { //将A[0, n)随机打乱
   while ( 1 < n )
      swap ( A[rand() % n], A[--n] );
}

void shuffle ( int A[], int lo, int hi ) //将A[lo, hi)随机打乱
{  shuffle ( A + lo, hi - lo );  }
