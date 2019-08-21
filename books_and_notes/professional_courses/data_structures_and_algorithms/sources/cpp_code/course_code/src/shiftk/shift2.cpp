/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "../reverse/reverse.h"
/*DSA*/#include "../UniPrint/print_int_array.h"

int shift2 ( int* A, int n, int k ) { //借助倒置算法，将数组循环左移k位，O(3n)
   k %= n; //确保k <= n
   reverse ( A, k ); //将区间A[0, k)倒置：O(3k/2)次操作
   /*DSA*/print ( A, n, 0, k );
   reverse ( A + k, n - k ); //将区间A[k, n)倒置：O(3(n - k)/2)次操作
   /*DSA*/print ( A, n, k, n );
   reverse ( A, n ); //倒置整个数组A[0, n)：O(3n/2)次操作
   /*DSA*/print ( A, n );
   return 3 * n; //返回累计操作次数，以便与其它算法比较：3/2 * (k + (n - k) + n) = 3n
}
