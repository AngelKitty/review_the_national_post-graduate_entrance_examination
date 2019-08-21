/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/******************************************************************************************
 * Test of Binary Search Tree
 ******************************************************************************************/
#include "BST_test.h"

/******************************************************************************************
 * Test a BST
 ******************************************************************************************/
template <typename T> void  testBST ( int n ) {
   BST<T> bst;
   while ( bst.size() < n ) bst.insert ( dice ( ( T ) n * 3 ) ); print ( bst ); //随机创建
   bst.stretchToLPath(); print ( bst ); //伸直成撇
   while ( !bst.empty() ) bst.remove ( bst.root()->data ); //清空
   while ( bst.size() < n ) bst.insert ( dice ( ( T ) n * 3 ) ); print ( bst ); //随机创建
   bst.stretchToRPath(); print ( bst ); //伸直成捺
   while ( !bst.empty() ) bst.remove ( bst.root()->data ); //清空
   while ( bst.size() < n ) { //随机插入、查询、删除
      T e = dice ( ( T ) n * 3 ); //[0, 3n)范围内的e
      switch ( dice ( 3 ) ) {
         case 0: { //查找，成功率 <= 33.3%
            printf ( "Searching for " ); print ( e ); printf ( " ... " );
            BinNodePosi(T) & p = bst.search ( e );
            p ?
            printf ( "Found with" ), print ( p->data ), printf ( "\n" ) :
            printf ( "not found\n" );
            break;
         }
         case 1: { //删除，成功率 <= 33.3%
            printf ( "Removing " ); print ( e ); printf ( " ... " );
            bst.remove ( e ) ?
            printf ( "Done\n" ), print ( bst ) :
            printf ( "not exists\n" );
            break;
         }
         default: {//插入，成功率 == 100%
            printf ( "Inserting " ); print ( e ); printf ( " ... " );
            printf ( "Done with" ), print ( bst.insert ( e )->data ), printf ( "\n" ), print ( bst );
            break;
         }
      }
   }
   while ( bst.size() > 0 ) { //清空
      T e = dice ( ( T ) n * 3 ); //[0, 3n)范围内的e
      printf ( "Removing " ); print ( e ); printf ( " ... " );
      bst.remove ( e ) ? printf ( "Done\n" ), print ( bst ) : printf ( "not exists\n" );
   }
}

/******************************************************************************************
 * 测试主入口
 ******************************************************************************************/
int main ( int argc, char* argv[] ) {
   if ( 2 > argc ) { printf ( "Usage: %s <size of test>\a\a\n", argv[0] ); return 1; }
   srand ( ( unsigned int ) time ( NULL ) );
   testBST<int> ( atoi ( argv[1] ) ); //元素类型可以在这里任意选择
   return 0;
}