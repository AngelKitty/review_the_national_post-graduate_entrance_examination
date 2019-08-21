/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/******************************************************************************************
* Test of B-Tree
******************************************************************************************/
#include "BTree_test.h"

/******************************************************************************************
* Test a BTree
******************************************************************************************/
template <typename T> void  testBTree ( int order, int n ) {
   BTree<T> bt ( order );
   while ( bt.size() < n ) {
      T e = dice ( ( T ) n * 3 ); //[0, 3n)范围内的e
      switch ( dice ( 3 ) ) {
         case 0: { //查找，成功率 <= 33.3%
            printf ( "Searching for " ); print ( e ); printf ( " ... " );
            BTNodePosi(T) p = bt.search ( e );
            printf ( p ? "Found\n" : "Not found\n" );
            break;
         }
         case 1: { //删除，成功率 <= 33.3%
            printf ( "Removing " ); print ( e ); printf ( " ... " );
            bt.remove ( e ) ?
            printf ( "Done\n" ), print ( bt ) :
            printf ( "Not exists\n" );
            break;
         }
         default: {//插入，成功率 == 100%
            printf ( "Inserting " ); print ( e ); printf ( " ... " );
            int oldSize = bt.size();
            bt.insert ( e ) ?
            printf ( "Done\n" ), print ( bt ) :
            printf ( "Dup key\n" );
            break;
         }
      }
   }
   while ( bt.size() > 0 ) {
      T e = dice ( ( T ) n * 3 ); //[0, 3n)范围内的e
      printf ( "Removing " ); print ( e ); printf ( " ... " );
      bt.remove ( e ) ?
      printf ( "Done\n" ), print ( bt ) :
      printf ( "not exists\n" );
   }
}

/******************************************************************************************
* 测试主入口
******************************************************************************************/
int main ( int argc, char* argv[] ) {
   if ( 3 > argc ) { printf ( "Usage: %s <order of B-tree> <size of test>\a\a\n", argv[0] ); return 1; }
   int order = atoi ( argv[1] ); if ( order < 3 )  { printf ( "Make sure the order (%d) is no less than 3.\a\a\n", order ); return 1; }
   int size = atoi ( argv[2] ); if ( size < 0 )  { printf ( "Make sure the size (%d) is no less than 0.\a\a\n", size ); return 1; }
   srand ( ( unsigned int ) time ( NULL ) );
   testBTree<int> ( atoi ( argv[1] ), atoi ( argv[2] ) ); //元素类型、比较器可以在这里任意选择
   return 0;
}