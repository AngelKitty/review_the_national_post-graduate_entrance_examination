/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/******************************************************************************************
 * Test of Vector
 ******************************************************************************************/
#include "cursor_list.h"

/******************************************************************************************
 * 测试游标式列表
 ******************************************************************************************/
void testCursor ( int testSize ) {
   CursorList<char> CL ( testSize ); CL.print();
   //CL.insert('A'); CL.insert('B'); CL.insert('C'); CL.insert('D'); CL.insert('E'); CL.print();
   //CL.remove(CL.find('A')); CL.remove(CL.find('B')); CL.remove(CL.find('C')); CL.remove(CL.find('D')); CL.remove(CL.find('E')); CL.print();
   //CL.insert('A'); CL.insert('B'); CL.insert('C'); CL.insert('D'); CL.insert('E'); CL.print();
   for ( int i = 0; i < testSize ; i++ ) {
      char x = 'A' + ( rand() % 26 );
      if ( 30 < ( rand() % 100 ) ) {
         printf ( "%c inserted at %d\n", x, CL.insert ( x ) );
      } else {
         if ( 0 > CL.find ( x ) )
            printf ( "%c not found\n", x );
         else
            printf ( "%c removed at %d\n", x, CL.remove ( CL.find ( x ) ) );
      }
      CL.print();
   }
   for ( char x = 'A'; x <= 'Z'; x++ ) {
      while ( 0 <= CL.find ( x ) )
         printf ( "%c removed at %d\n", x, CL.remove ( CL.find ( x ) ) );
      CL.print();
   }
}

/******************************************************************************************
 * 测试向量
 ******************************************************************************************/
int main ( int argc, char* argv[] ) {
   if ( 2 > argc ) { printf ( "Usage: %s <size of test>\a\a\n", argv[0] ); return 1; }
   srand ( ( unsigned int ) time ( NULL ) ); //设置随机种子
   testCursor ( atoi ( argv[1] ) ); //元素类型可以在这里任意选择
   return 0;
}