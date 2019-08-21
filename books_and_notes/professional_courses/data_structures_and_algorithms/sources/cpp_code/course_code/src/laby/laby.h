/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

// 迷宫寻径算法：在格单元s至t之间规划一条通路（如果的确存在）
bool labyrinth ( Cell Laby[LABY_MAX][LABY_MAX], Cell* s, Cell* t ) {
   if ( ( AVAILABLE != s->status ) || ( AVAILABLE != t->status ) ) return false; //退化情况
   Stack<Cell*> path; //用栈记录通路（Theseus的线绳）
   s->incoming = UNKNOWN; s->status = ROUTE; path.push ( s ); //起点
   do { //从起点出发不断试探、回溯，直到抵达终点，或者穷尽所有可能
      /*DSA*/displayLaby(); /*path.traverse(printLabyCell); printLabyCell(path.top());*/ getchar();
      Cell* c = path.top(); //检查当前位置（栈顶）
      if ( c == t ) return true; //若已抵达终点，则找到了一条通路；否则，沿尚未试探的方向继续试探
      while ( NO_WAY > ( c->outgoing = nextESWN ( c->outgoing ) ) ) //逐一检查所有方向
         if ( AVAILABLE == neighbor ( c )->status ) break; //试图找到尚未试探的方向
      if ( NO_WAY <= c->outgoing ) //若所有方向都已尝试过
         { c->status = BACKTRACKED; c = path.pop(); }//则向后回溯一步
      else //否则，向前试探一步
         { path.push ( c = advance ( c ) ); c->outgoing = UNKNOWN; c->status = ROUTE; }
   } while ( !path.empty() );
   return false;
}