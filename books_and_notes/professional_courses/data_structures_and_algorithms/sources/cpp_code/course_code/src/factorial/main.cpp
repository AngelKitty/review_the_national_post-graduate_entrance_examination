/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "fac.h"

int main ( int argc, char* argv[] ) { //Ω◊≥À‘ÀÀ„≤‚ ‘
   for ( int i = 0; i < 25; i++ )
      printf ( "\t%d\t%22I64d\t%22I64d\n", i, facI ( i ), facR ( i ) );
   return 0;
}