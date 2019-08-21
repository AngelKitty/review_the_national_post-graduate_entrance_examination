/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/******************************************************************************************
 * Test of AVL Tree
 ******************************************************************************************/
#include "AVL_test.h"

/******************************************************************************************
 * Test an AVL
 ******************************************************************************************/
template <typename T> void  testAVL ( int n ) {
   AVL<T> avl;
   while ( avl.size() < n ) {
      T e = dice ( ( T ) n * 3 ); //[0, 3n)范围内的e
      switch ( dice ( 3 ) ) {
         case 0: { //查找，成功率 <= 33.3%
            printf ( "Searching for " ); print ( e ); printf ( " ...\n" );
            BinNodePosi(T) & p = avl.search ( e );
            p ?
            printf ( "Found with" ), print ( p ), printf ( "\n" ) :
            printf ( "Not found\n" );
            break;
         }
         case 1: { //删除，成功率 <= 33.3%
            printf ( "Removing " ); print ( e ); printf ( " ...\n" );
            avl.remove ( e ) ? printf ( "Done\n" ), print ( avl ) : printf ( "Not exists\n" );
            break;
         }
         default: {//插入，成功率 == 100%
            printf ( "Inserting " ); print ( e ); printf ( " ...\n" );
            BinNodePosi(T) p = avl.insert ( e );
            if ( p->data != e ) { print ( p->data ); printf ( " <> " ); print ( e ); printf ( "\n" ); }
            printf ( "Done with" ), print ( p ), printf ( "\n" ), print ( avl );
            break;
         }
      }
   }
   while ( avl.size() > 0 ) {
      T e = dice ( ( T ) n * 3 ); //[0, 3n)范围内的e
      printf ( "Removing " ); print ( e ); printf ( " ...\n" );
      avl.remove ( e ) ? printf ( "Done\n" ), print ( avl ) : printf ( "Not exists\n" );
   }
}

/******************************************************************************************
 * 测试主入口
 ******************************************************************************************/
int main ( int argc, char* argv[] ) {
   if ( 2 > argc ) { printf ( "Usage: %s <size of test>\a\a\n", argv[0] ); return 1; }
   srand ( ( unsigned int ) time ( NULL ) );
   testAVL<int> ( atoi ( argv[1] ) ); //元素类型可以在这里任意选择
   return 0;
}