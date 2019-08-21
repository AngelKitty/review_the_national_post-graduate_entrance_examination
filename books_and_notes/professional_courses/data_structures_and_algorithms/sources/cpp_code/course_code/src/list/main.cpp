/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/******************************************************************************************
 * Test of list
 ******************************************************************************************/
#include "list_test.h"

int testID = 0; //测试编号

/******************************************************************************************
 * 随机生成长度为n的列表（其中可能包含重复节点）
 ******************************************************************************************/
template <typename T> //元素类型
void randomList ( List<T> & list, int n ) { //在[0, 2n)中选择n个偶数，随机插入列表
   ListNodePosi(T) p =
      ( rand() % 2 ) ?
      list.insertAsLast ( rand() % ( T ) n * 2 ) :
      list.insertAsFirst ( rand() % ( T ) n * 2 );
   for ( int i = 1; i < n; i++ )
      p = rand() % 2 ?
          list.insertB ( p, rand() % ( T ) n * 2 ) :
          list.insertA ( p, rand() % ( T ) n * 2 );
}

/******************************************************************************************
 * 测试列表
 ******************************************************************************************/
#define PRINT(x)  { print(x); crc(x); checkOrder(x); }
template <typename T> //元素类型
void   testList ( int testSize ) {
   printf ( "\n  ==== Test %2d. Generate two lists each of size %d by random insertions\n", testID++, testSize );
   List<T> La; randomList ( La, testSize ); PRINT ( La );
   List<T> Lb; randomList ( Lb, testSize ); PRINT ( Lb );
   printf ( "\n  ==== Test %2d. Call list members by rank (with high complexity)\n", testID++ );
   for ( int i = 0; i < La.size(); i++ ) print ( La[i] ); printf ( "\n" );
   for ( int i = 0; i < Lb.size(); i++ ) print ( Lb[i] ); printf ( "\n" );
   printf ( "\n  ==== Test %2d. Concatenation\n", testID++ ); PRINT ( La ); PRINT ( Lb );
   while ( 0 < Lb.size() ) La.insertAsLast ( Lb.remove ( Lb.first() ) ); PRINT ( La ); PRINT ( Lb );
   printf ( "\n  ==== Test %2d. Increase\n", testID++ ); PRINT ( La );
   increase ( La ); PRINT ( La );
   printf ( "\n  ==== Test %2d. Lowpass (with high complexity) on\n", testID++ ); PRINT ( La );
   int i = La.size(); while ( 0 < --i ) { La[i-1] += La[i]; La[i-1] >>= 1; } PRINT ( La );
   printf ( "\n  ==== Test %2d. reverse\n", testID++, testSize );
   La.reverse(); PRINT ( La );
   printf ( "\n  ==== Test %2d. Copy\n", testID++ ); PRINT ( La );
   List<T> Ld ( La ); PRINT ( Ld );
   printf ( "\n  ==== Test %2d. Trim by random deletions\n", testID++ ); PRINT ( Ld );
   while ( testSize / 4 < Ld.size() ) {
      int N = rand() % Ld.size(); printf ( "removing L[%d]=", N );
      ListNodePosi(T) p = Ld.first(); while ( 0 < N-- ) p = p->succ;
      print ( p->data ); printf ( " ...\n" );
      Ld.remove ( p ); PRINT ( Ld );
   }
   printf ( "\n  ==== Test %2d. Copy\n", testID++ ); PRINT ( La );
   List<T> Le ( La ); PRINT ( Le );
   printf ( "\n  ==== Test %2d. FIND in\n", testID++ ); PRINT ( Le );
   for ( int i = 0; i <= testSize * 2; i++ ) { //逐一测试[0, 2n]中的所有可能
      ListNodePosi(T) p = Le.find ( ( T ) i ); printf ( "Looking for " ); print ( ( T ) i ); printf ( ": " );
      if ( p ) { printf ( " found with" ); print ( p->data ); }
      else printf ( " not found" );
      printf ( "\n" );
   } //正确的结构应该是大致（n+1次）失败、（n次）成功相间
   printf ( "\n  ==== Test %2d. Sort\n", testID++ ); PRINT ( La );
   La.sort(); PRINT ( La );
   printf ( "\n  ==== Test %2d. SEARCH in\n", testID++ ); PRINT ( La );
   for ( int i = 0; i <= testSize * 2; i++ ) { //逐一测试[0, 2n]中的所有可能
      ListNodePosi(T) p = La.search ( ( T ) i ); printf ( "Looking for " ); print ( ( T ) i ); printf ( ": " );
      printf ( " stopped at" ); print ( p->data );
      if ( ( T ) i == p->data ) printf ( " and found" );
      printf ( "\n" );
   } //正确的结构应该是大致（n+1次）失败、（n次）成功相间
   printf ( "\n  ==== Test %2d. Remove redundancy in\n", testID++ ); PRINT ( La );
   printf ( "%d node(s) removed\n", La.uniquify() ); PRINT ( La ); La.reverse(); PRINT ( La );
   printf ( "\n  ==== Test %2d. Remove redundancy in\n", testID++ ); PRINT ( Le );
   printf ( "%d node(s) removed\n", Le.deduplicate() ); PRINT ( Le );
   printf ( "\n  ==== Test %2d. Sort\n", testID++ ); PRINT ( Le );
   Le.sort(); PRINT ( Le );
   return;
}

/******************************************************************************************
 * 测试列表
 ******************************************************************************************/
int main ( int argc, char* argv[] ) {
   if ( 2 > argc ) { printf ( "Usage: %s <size of test>\a\a\n", argv[0] ); return 1; }
   srand ( ( unsigned int ) time ( NULL ) );
   testList<int> ( atoi ( argv[1] ) ); //元素类型可以在这里任意选择
   return 0;
}