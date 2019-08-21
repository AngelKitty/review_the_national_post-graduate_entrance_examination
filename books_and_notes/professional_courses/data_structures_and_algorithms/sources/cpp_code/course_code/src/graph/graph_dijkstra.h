/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename Tv, typename Te> //最短路径Dijkstra算法：适用于一般的有向图
void Graph<Tv, Te>::dijkstra ( int s ) { //assert: 0 <= s < n
   reset(); priority ( s ) = 0;
   for ( int i = 0; i < n; i++ ) { //共需引入n个顶点和n-1条边
      status ( s ) = VISITED;
      if ( -1 != parent ( s ) ) type ( parent ( s ), s ) = TREE; //引入当前的s
      for ( int j = firstNbr ( s ); -1 < j; j = nextNbr ( s, j ) ) //枚举s的所有邻居j
         if ( ( status ( j ) == UNDISCOVERED ) && ( priority ( j ) > priority ( s ) + weight ( s, j ) ) ) //对邻接顶点j做松弛
            { priority ( j ) = priority ( s ) + weight ( s, j ); parent ( j ) = s; } //与Prim算法唯一的不同之处
      for ( int shortest = INT_MAX, j = 0; j < n; j++ ) //选出下一最近顶点
         if ( ( status ( j ) == UNDISCOVERED ) && ( shortest > priority ( j ) ) )
            { shortest = priority ( j ); s = j; }
   }
} //对于无向连通图，假设每一条边表示为方向互逆、权重相等的一对边