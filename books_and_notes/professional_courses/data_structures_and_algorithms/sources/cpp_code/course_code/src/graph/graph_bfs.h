/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename Tv, typename Te> //广度优先搜索BFS算法（全图）
void Graph<Tv, Te>::bfs ( int s ) { //assert: 0 <= s < n
   reset(); int clock = 0; int v = s; //初始化
   do //逐一检查所有顶点
      if ( UNDISCOVERED == status ( v ) ) //一旦遇到尚未发现的顶点
         BFS ( v, clock ); //即从该顶点出发启动一次BFS
   while ( s != ( v = ( ++v % n ) ) ); //按序号检查，故不漏不重
}

template <typename Tv, typename Te> //广度优先搜索BFS算法（单个连通域）
void Graph<Tv, Te>::BFS ( int v, int& clock ) { //assert: 0 <= v < n
   Queue<int> Q; //引入辅助队列
   status ( v ) = DISCOVERED; Q.enqueue ( v ); //初始化起点
   while ( !Q.empty() ) { //在Q变空之前，不断
      int v = Q.dequeue(); dTime ( v ) = ++clock; //取出队首顶点v
      for ( int u = firstNbr ( v ); -1 < u; u = nextNbr ( v, u ) ) //枚举v的所有邻居u
         if ( UNDISCOVERED == status ( u ) ) { //若u尚未被发现，则
            status ( u ) = DISCOVERED; Q.enqueue ( u ); //发现该顶点
            type ( v, u ) = TREE; parent ( u ) = v; //引入树边拓展支撑树
         } else { //若u已被发现，或者甚至已访问完毕，则
            type ( v, u ) = CROSS; //将(v, u)归类于跨边
         }
      status ( v ) = VISITED; //至此，当前顶点访问完毕
   }
}