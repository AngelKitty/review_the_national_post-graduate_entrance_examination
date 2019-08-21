/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename Tv, typename Te> void Graph<Tv, Te>::bcc ( int s ) { //基于DFS的BCC分解算法
   reset(); int clock = 0; int v = s; Stack<int> S; //栈S用以记录已访问的顶点
   do
      if ( UNDISCOVERED == status ( v ) ) { //一旦发现未发现的顶点（新连通分量）
         BCC ( v, clock, S ); //即从该顶点出发启动一次BCC
         S.pop(); //遍历返回后，弹出栈中最后一个顶点——当前连通域的起点
      }
   while ( s != ( v = ( ++v % n ) ) );
}
#define hca(x) (fTime(x)) //利用此处闲置的fTime[]充当hca[]
template <typename Tv, typename Te> //顶点类型、边类型
void Graph<Tv, Te>::BCC ( int v, int& clock, Stack<int>& S ) { //assert: 0 <= v < n
   hca ( v ) = dTime ( v ) = ++clock; status ( v ) = DISCOVERED; S.push ( v ); //v被发现并入栈
   for ( int u = firstNbr ( v ); -1 < u; u = nextNbr ( v, u ) ) //枚举v的所有邻居u
      switch ( status ( u ) ) { //并视u的状态分别处理
         case UNDISCOVERED:
            parent ( u ) = v; type ( v, u ) = TREE; BCC ( u, clock, S ); //从顶点u处深入
            if ( hca ( u ) < dTime ( v ) ) //遍历返回后，若发现u（通过后向边）可指向v的真祖先
               hca ( v ) = min ( hca ( v ), hca ( u ) ); //则v亦必如此
            else { //否则，以v为关节点（u以下即是一个BCC，且其中顶点此时正集中于栈S的顶部）
               /*DSA*/printf ( "BCC rooted at %c:", vertex ( v ) );
               /*DSA*/Stack<int> temp; do { temp.push ( S.pop() ); print ( vertex ( temp.top() ) ); } while ( v != temp.top() ); while ( !temp.empty() ) S.push ( temp.pop() );
               while ( v != S.pop() ); //依次弹出当前BCC中的节点，亦可根据实际需求转存至其它结构
               S.push ( v ); //最后一个顶点（关节点）重新入栈——分摊不足一次
               /*DSA*/printf ( "\n" );
            }
            break;
         case DISCOVERED:
            type ( v, u ) = BACKWARD; //标记(v, u)，并按照“越小越高”的准则
            if ( u != parent ( v ) ) hca ( v ) = min ( hca ( v ), dTime ( u ) ); //更新hca[v]
            break;
         default: //VISITED (digraphs only)
            type ( v, u ) = ( dTime ( v ) < dTime ( u ) ) ? FORWARD : CROSS;
            break;
      }
   status ( v ) = VISITED; //对v的访问结束
}
#undef hca