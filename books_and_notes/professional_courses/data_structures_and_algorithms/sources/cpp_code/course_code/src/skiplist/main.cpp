/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/******************************************************************************************
 * Test of Skiplist
 ******************************************************************************************/

#include "skipList_test.h"

/******************************************************************************************
 * 测试跳转表
 ******************************************************************************************/
template <typename K, typename V> //key、value
void testSkiplist ( int n ) {
   Skiplist<K, V> L;
   while ( L.size() < n )
      switch ( dice ( 3 ) ) {
         case 0: { //查找，成功率 <= 33.3%
            K key = dice ( ( K ) n * 3 ); //[0, 3n)范围内的key
            printf ( "Searching for " ); print ( key ); printf ( " ... " );
            V* pValue = L.get ( key );
            pValue ? printf ( "found with " ), print ( *pValue ) : printf ( "Not found" ); printf ( "\n" );
            break;
         }
         case 1: { //删除，成功率 <= 33.3%
            K key = dice ( ( K ) n * 3 ); //[0, 3n)范围内的key
            printf ( "Removing " ); print ( key ); printf ( " ... " );
            L.remove ( key ) ? printf ( "Done\n" ) : printf ( "Entry not exists\n" );
            print ( L );
            break;
         }
         default: {//插入，成功率 == 100%
            K k = dice ( ( K ) n * 3 ); V v = ( V ) 'A' + dice ( 26 ); //在[0, 2n)*['A'~'Z']范围内的词条
            printf ( "Inserting <" ); print ( k ); printf ( "," ); print ( v ); printf ( "> ... " );
            L.put ( k, v ); printf ( "Done\n" );
            print ( L );
            break;
         }
      }
   while ( L.size() > 0 ) {
      K key = dice ( ( K ) n * 3 ); //[0, 3n)范围内的key
      printf ( "Removing " ); print ( key ); printf ( " ... " );
      L.remove ( key ) ? printf ( "Done\n" ) : printf ( "Entry not exists\n" );
      print ( L );
   }
}

/******************************************************************************************
 * 主程序
 ******************************************************************************************/
int main ( int argc, char* argv[] ) {
   if ( 2 > argc ) { printf ( "Usage: %s <size of test>\a\a\n", argv[0] ); return 1; }
   srand ( ( unsigned int ) time ( NULL ) );
   testSkiplist<int, char> ( atoi ( argv[1] ) ); //元素类型可以在这里任意选择
   return 0;
}