/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "../Bitmap/Bitmap.h"

int primeNLT ( int c, int n, char* file ) { //根据file文件中的记录，在[c, n)内取最小的素数
   Bitmap B ( file, n ); //file已经按位图格式，记录了n以内的所有素数，因此只要
   while ( c < n ) //从c开始，逐位地
      if ( B.test ( c ) ) c++; //测试，即可
      else return c; //返回首个发现的素数
   return c; //若没有这样的素数，返回n（实用中不能如此简化处理）
}
