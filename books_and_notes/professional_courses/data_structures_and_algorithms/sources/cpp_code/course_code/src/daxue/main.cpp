/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#include<iostream>
using namespace std;

#define nStep  8
char* level[8] =
{"明明德於天下", "治國", "齊家", "修身", "正心", "誠意", "致知", "格物"};

void phase ( int n, bool done ) {
   for ( int i = 0; i < n; i++ )  cout << "\t│";
   cout << "\t" << ( done ? "└" : "┌" ) << level[n] << endl;
}

void towardsDaxue ( int n ) {
   phase ( n, false );
   if ( n + 1 < nStep ) towardsDaxue ( n + 1 );
   phase ( n, true );
}

int main ( char* argv[], int argc ) {
   towardsDaxue ( 0 );
   return 0;
}