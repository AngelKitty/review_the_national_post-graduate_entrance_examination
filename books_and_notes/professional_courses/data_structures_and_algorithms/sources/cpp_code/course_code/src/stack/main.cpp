/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/******************************************************************************************
 * Test of stack
 ******************************************************************************************/
#include "stack_test.h"

int testID = 0; //测试编号

/******************************************************************************************
 * 测试栈
 ******************************************************************************************/
template <typename T> //元素类型
void testStack ( int n ) {
   Stack<T> S;
   printf ( "\n  ==== Test %2d. Growing stack\n", testID++ );
   while ( S.size() < n ) {
      T e = rand() % ( T ) 2 * n;
      if ( S.empty() || ( 30 < ( rand() % 100 ) ) ) { //70%概率入栈
         printf ( "pushing " ); print ( e ); printf ( " ...\n" );
         S.push ( e );
      } else { //30%概率出栈
         printf ( "poping with ... " ); print ( S.pop() ); printf ( "\n" );
      }
      print ( S );
   }
   printf ( "\n  ==== Test %2d. Shrinking stack\n", testID++ );
   while ( !S.empty() ) {
      T e = dice ( ( T ) 2 * n );
      if ( 70 < dice ( 100 ) ) { //70%概率入栈
         printf ( "pushing " ); print ( e ); printf ( " ...\n" );
         S.push ( e );
      } else { //70%概率出栈
         printf ( "poping with ... " ); print ( S.pop() ); printf ( "\n" );
      }
      print ( S );
   }
}


/******************************************************************************************
 * 测试栈
 ******************************************************************************************/
int main ( int argc, char* argv[] ) {
   if ( 2 > argc ) { printf ( "Usage: %s <size of test>\a\a\n", argv[0] ); return 1; }
   srand ( ( unsigned int ) time ( NULL ) ); //设置随机种子
   testStack<int> ( atoi ( argv[1] ) ); //元素类型可以在这里任意选择
   return 0;
}