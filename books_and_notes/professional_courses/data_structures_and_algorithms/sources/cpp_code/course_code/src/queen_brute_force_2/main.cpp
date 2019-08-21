/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#include   "../queen_brute_force_1/queen.h"

/******************************************************************************************
 * 全局变量
 ******************************************************************************************/
RunMode runMode; //演示模式
int nSolu = 0; //解的总数
int nCheck = 0; //尝试的总次数

/******************************************************************************************
 * 测试入口
 ******************************************************************************************/
int main ( int argc, char* argv[] ) {
   void ( *pPlaceQueens[] ) ( void ) = {
      place2Queens, //2皇后算法
      place3Queens, //3皇后算法
      place4Queens, //4皇后算法
      place5Queens  //5皇后算法
   };
   if ( argc < 2 ) //检查命令行格式
      { cout << "Usage: " << argv[0] << " <-step | -cont>" << endl; return -1; }
   if ( !strcmp ( "-step", argv[1] ) ) runMode = Step; //设定为单步演示模式
   else if ( !strcmp ( "-cont", argv[1] ) ) runMode = Continuous; //设定为连续演示模式
   else { cout << "Unrecognizable running mode" << endl; return -2; }
   for ( int i = 2; i <= 5; i++ ) { //对于2~5个皇后的情况，分别
      nSolu = 0; nCheck = 0; //初始化计数器
      ( *pPlaceQueens[i-2] ) (); //启动算法
      printf ( "#Queen = %d\t#Solution = %d\t#Check = %d\a\a\n", i, nSolu, nCheck ); //输出解
      getchar();
   }
}