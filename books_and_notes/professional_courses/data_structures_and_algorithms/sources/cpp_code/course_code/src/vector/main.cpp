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
#include "vector_test.h"

int testID = 0; //测试编号

/******************************************************************************************
 * 测试：无序向量的（顺序）查找
 ******************************************************************************************/
template <typename T> //元素类型
void TestFind ( Vector<T> & V, int n ) {
   for ( int i = 0; i < V.size(); i++ ) { //依次查找向量中元素，当然成功
      T e =  V[i]; print ( e );
      Rank r = V.find ( e );
      if ( r < 0 ) printf ( " : not found until rank V[%d] <> %d", r, e );
      else printf ( " : found at rank V[%d] = %d", r, V[r] );
      printf ( "\n" );
   }
   for ( int i = 0; i <= V.size(); i++ ) { //依次查找每对相邻元素的均值，可能成功
      T a = ( 0 < i ) ? V[i-1] : -INT_MAX / 2;
      T b = ( i < V.size() ) ? V[i] : INT_MAX / 2;
      T e =  ( a + b ) / 2; print ( e );
      Rank r = V.find ( e );
      if ( r < 0 ) printf ( " : not found until rank V[%d] <> %d", r, e );
      else printf ( " : found at rank V[%d] = %d", r, V[r] );
      printf ( "\n" );
   }
}

/******************************************************************************************
 * 测试：有序向量的查找（binSearch或fibSearch）
 ******************************************************************************************/
template <typename T> //元素类型
void TestSearch ( Vector<T> & V ) {
   for ( int i = 0; i < V.size(); i++ ) { //依次查找向量中元素，当然成功
      T e =  V[i]; printf ( "Looking for" ); print ( e ); printf ( " in ...\n" ); print ( V );
      Rank r = V.search ( e );
      if ( V[r] == e ) printf ( "found at rank V[%d] = %d", r, V[r] );
      else printf ( "found at rank V[%d] = %d <> %d\a\a", r, V[r], e );
      printf ( "\n\n" );
   }
   for ( int i = 0; i <= V.size(); i++ ) { //依次相邻元素的均值，可能成功
      T a = ( 0 < i ) ? V[i - 1] : -INT_MAX / 2;
      T b = ( i < V.size() ) ? V[i] : INT_MAX / 2;
      T e =  ( a + b ) / 2; printf ( "Looking for" ); print ( e ); printf ( " in ...\n" ); print ( V );
      Rank r = V.search ( e );
      printf ( "V[%3d] =", r ); ( r < 0 ) ? print ( "-INF" ) : print ( V[r] ); printf ( "  ~  " );
      printf ( "V[%3d] =", r + 1 ); ( r + 1 < V.size() ) ? print ( V[r + 1] ) : print ( "+INF" );
      bool ordered = true;
      if ( ( r >= 0 ) && ( V[r] > e ) ) ordered = false;
      if ( ( r + 1 < V.size() ) && ( V[r + 1] <= e ) ) ordered = false;
      if ( !ordered ) printf ( "\tincorrect search\a\a" );
      printf ( "\n\n" );
   }
}

/******************************************************************************************
 * 测试：有序向量的插入
 ******************************************************************************************/
template <typename T> //元素类型
void TestOrderedInsertion ( Vector<T> & V, int n ) {
   while ( n * 2 > V.size() ) {
      T e = dice ( ( T ) n * 2 );
      printf ( "Inserting " ); print ( e ); printf ( " ...\n" );
      V.insert ( V.search ( e ) + 1, e );
      print ( V );
   }
}

/******************************************************************************************
 * 测试向量
 ******************************************************************************************/
#define PRINT(x)  { print(x); crc(x); checkOrder(x); }
template <typename T> //元素类型
void   testVector ( int testSize ) {
   printf ( "\n  ==== Test %2d. Generate a random vector\n", testID++ );
   Vector<T> V;
   for ( int i = 0; i < testSize; i++ ) V.insert ( dice ( i + 1 ), dice ( ( T ) testSize * 3 ) ); //在[0, 3n)中选择n个数，随机插入向量
   PRINT ( V );
   permute ( V );
   PRINT ( V )
   printf ( "\n  ==== Test %2d. Lowpass on\n", testID++ ); PRINT ( V );
   int i = V.size(); while ( 0 < --i ) { V[i-1] += V[i]; V[i-1] >>= 1; }    PRINT ( V );
   printf ( "\n  ==== Test %2d. Increase\n", testID++ ); PRINT ( V );
   increase ( V ); PRINT ( V );
   printf ( "\n  ==== Test %2d. FIND in\n", testID++ ); PRINT ( V );
   TestFind<T> ( V, testSize );
   printf ( "\n  ==== Test %2d. Sort degenerate intervals each of size 1 in\n", testID++, 0, V.size() ); PRINT ( V );
   for ( int i = 0; i < V.size(); i += V.size() / 5 ) { V.sort ( i, i ); PRINT ( V ); } //element by element
   printf ( "\n  ==== Test %2d. Sort 5 intervals each of size %d in\n", testID++, V.size() / 5 ); PRINT ( V );
   for ( int i = 0; i < V.size(); i += V.size() / 5 ) { V.sort ( i, min ( V.size(), i + V.size() / 5 ) ); PRINT ( V ); } //interval by interval
   printf ( "\n  ==== Test %2d. Sort the entire vector of\n", testID++, 0, V.size() ); PRINT ( V );
   V.sort();   PRINT ( V );
   printf ( "\n  ==== Test %2d. FIND in\n", testID++ ); PRINT ( V );
   TestFind<T> ( V, testSize );
   printf ( "\n  ==== Test %2d. SEARCH in\n", testID++ ); PRINT ( V );
   TestSearch<T> ( V );
   printf ( "\n  ==== Test %2d. Unsort interval [%d, %d) in\n", testID++, V.size() / 4, 3 * V.size() / 4 ); PRINT ( V );
   V.unsort ( V.size() / 4, 3 * V.size() / 4 );   PRINT ( V );
   printf ( "\n  ==== Test %2d. Unsort interval [%d, %d) in\n", testID++, 0, V.size() ); PRINT ( V );
   V.unsort();   PRINT ( V );
   printf ( "\n  ==== Test %2d. Copy interval [%d, %d) from\n", testID++, V.size() / 4, 3 * V.size() / 4 ); PRINT ( V );
   Vector<T> U ( V, V.size() / 4, 3 * V.size() / 4 );
   PRINT ( U );
   printf ( "\n  ==== Test %2d. Copy from\n", testID++ ); PRINT ( V );
   Vector<T> W ( V );
   PRINT ( W );
   printf ( "\n  ==== Test %2d. Clone from\n", testID++ ); PRINT ( U );
   W = U;
   PRINT ( W );
   printf ( "\n  ==== Test %2d. Remove redundancy in unsorted\n", testID++ ); PRINT ( V );
   printf ( "%d node(s) removed\n", V.deduplicate() );   PRINT ( V );
   printf ( "\n  ==== Test %2d. Sort interval [%d, %d) in\n", testID++, 0, V.size() ); PRINT ( V );
   V.sort();   PRINT ( V );
   printf ( "\n  ==== Test %2d. FIND in V[%d]\n", testID++ ); PRINT ( V );
   TestFind<T> ( V, testSize );
   printf ( "\n  ==== Test %2d. SEARCH & INSERT in\n", testID++ ); PRINT ( V );
   TestOrderedInsertion<T> ( V, testSize );  PRINT ( V );
   printf ( "\n  ==== Test %2d. Remove redundancy in sorted\n", testID++ ); PRINT ( V );
   printf ( "%d node(s) removed\n", V.uniquify() );   PRINT ( V );
}

/******************************************************************************************
 * 测试向量
 ******************************************************************************************/
int main ( int argc, char* argv[] ) {
   if ( 2 > argc ) { printf ( "Usage: %s <size of test>\a\a\n", argv[0] ); return 1; }
   srand ( ( unsigned int ) time ( NULL ) ); //设置随机种子
   testVector<int> ( atoi ( argv[1] ) ); //元素类型可以在这里任意选择
   return 0;
}