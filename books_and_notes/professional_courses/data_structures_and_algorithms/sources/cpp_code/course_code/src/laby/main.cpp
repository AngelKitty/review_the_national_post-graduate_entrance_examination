/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#include "../_share/util.h"
#include "../stack/stack.h" //使用栈
#include "Cell.h" //迷宫格点类

int labySize;
Cell* startCell;
Cell* goalCell;

#include "neighbor.h"
#include "advance.h"

#include "displayLaby.h"
#include "laby.h"

#include "randLaby.h"
#include "readLaby.h"

/******************************************************************************************
 * 迷宫寻径
 ******************************************************************************************/
int main ( int argc, char* argv[] ) {
   srand ( ( unsigned int ) time ( NULL ) ); //设置随机种子
   ( 1 < argc ) ? readLaby ( argv[1] ) : randLaby(); //使用指定迷宫文件，或随机生成
   labyrinth ( laby, startCell, goalCell ) ? //启动算法
   printf ( "\nRoute found\a\n" ) :
   printf ( "\nNo route found\a\n" );
   getchar();
   return 0;
}