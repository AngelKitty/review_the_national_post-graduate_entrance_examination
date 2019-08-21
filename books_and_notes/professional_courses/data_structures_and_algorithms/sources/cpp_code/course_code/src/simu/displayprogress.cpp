/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#include "simu.h"

/******************************************************************************************
 * 显示当前各（窗口）队列情况
 ******************************************************************************************/
void displayProgress ( Queue<Customer> windows[], int nWin, int now ) {
   system ( "cls" ); //清屏
   printf ( "============ Time: %4d ============\n", now );
   for ( int i = 0; i < nWin; i++ ) { //对每个窗口，分别
      printf ( "Window #%c:  ", 'A' + i ); //输出窗口编号
      windows[i].traverse ( printCustomer ); printf ( "\n" ); //列出队列中的所有顾客
   }
}