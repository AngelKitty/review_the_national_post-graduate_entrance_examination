/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename NodePosi> inline //在节点*p与*lc（可能为空）之间建立父（左）子关系
void attachAsLChild ( NodePosi p, NodePosi lc ) { p->lc = lc; if ( lc ) lc->parent = p; }

template <typename NodePosi> inline //在节点*p与*rc（可能为空）之间建立父（右）子关系
void attachAsRChild ( NodePosi p, NodePosi rc ) { p->rc = rc; if ( rc ) rc->parent = p; }

template <typename T> //Splay树伸展算法：从节点v出发逐层伸展
BinNodePosi(T) Splay<T>::splay ( BinNodePosi(T) v ) { //v为因最近访问而需伸展的节点位置
   if ( !v ) return NULL; BinNodePosi(T) p; BinNodePosi(T) g; //*v的父亲与祖父
   while ( ( p = v->parent ) && ( g = p->parent ) ) { //自下而上，反复对*v做双层伸展
      BinNodePosi(T) gg = g->parent; //每轮之后*v都以原曾祖父（great-grand parent）为父
      if ( IsLChild ( *v ) )
         if ( IsLChild ( *p ) ) { //zig-zig
            /*DSA*/printf ( "\tzIg-zIg :" ); print ( g ); print ( p ); print ( v ); printf ( "\n" );
            attachAsLChild ( g, p->rc ); attachAsLChild ( p, v->rc );
            attachAsRChild ( p, g ); attachAsRChild ( v, p );
         } else { //zig-zag
            /*DSA*/printf ( "\tzIg-zAg :" ); print ( g ); print ( p ); print ( v ); printf ( "\n" );
            attachAsLChild ( p, v->rc ); attachAsRChild ( g, v->lc );
            attachAsLChild ( v, g ); attachAsRChild ( v, p );
         }
      else if ( IsRChild ( *p ) ) { //zag-zag
         /*DSA*/printf ( "\tzAg-zAg :" ); print ( g ); print ( p ); print ( v ); printf ( "\n" );
         attachAsRChild ( g, p->lc ); attachAsRChild ( p, v->lc );
         attachAsLChild ( p, g ); attachAsLChild ( v, p );
      } else { //zag-zig
         /*DSA*/printf ( "\tzAg-zIg :" ); print ( g ); print ( p ); print ( v ); printf ( "\n" );
         attachAsRChild ( p, v->lc ); attachAsLChild ( g, v->rc );
         attachAsRChild ( v, g ); attachAsLChild ( v, p );
      }
      if ( !gg ) v->parent = NULL; //若*v原先的曾祖父*gg不存在，则*v现在应为树根
      else //否则，*gg此后应该以*v作为左或右孩子
         ( g == gg->lc ) ? attachAsLChild ( gg, v ) : attachAsRChild ( gg, v );
      updateHeight ( g ); updateHeight ( p ); updateHeight ( v );
   } //双层伸展结束时，必有g == NULL，但p可能非空
   if ( p = v->parent ) { //若p果真非空，则额外再做一次单旋
      /*DSA*/if ( IsLChild ( *v ) ) { printf ( "\tzIg :" ); print ( p ); print ( v ); printf ( "\n" ); }
      /*DSA*/else              { printf ( "\tzAg :" ); print ( p ); print ( v ); printf ( "\n" ); }
      if ( IsLChild ( *v ) ) { attachAsLChild ( p, v->rc ); attachAsRChild ( v, p ); }
      else                   { attachAsRChild ( p, v->lc ); attachAsLChild ( v, p ); }
      updateHeight ( p ); updateHeight ( v );
   }
   v->parent = NULL; return v;
} //调整之后新树根应为被伸展的节点，故返回该节点的位置以便上层函数更新树根