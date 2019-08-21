/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

/******************************************************************************************
 * Quadlist
 ******************************************************************************************/

template <typename T> //元素类型
void UniPrint::p ( Quadlist<T>& q ) { //引用
   printf ( "%s[%d]*%03d: ", typeid ( q ).name(), &q, q.size() ); //基本信息
   if ( q.size() <= 0 ) {  printf ( "\n" ); return;  }
   QuadlistNode<T>* curr = q.first()->pred; //当前层之header
   QuadlistNode<T>* base = q.first(); //当前节点所在
   while ( base->below ) base = base->below; //塔底
   while ( base->pred ) base = base->pred; //底层之header
   for ( int i = 0; i < q.size(); i++ ) { //对于当前层的每一节点
      curr = curr->succ; //curr
      QuadlistNode<T>* proj = curr; //找到与curr对应的
      while ( proj->below ) proj = proj->below; //塔底节点（投影）
      while ( ( base = base->succ ) != proj ) //移动base直到proj，期间不断
         printf ( "------------" ); //延长水平联接线
      print ( curr->entry ); //最后，输出当前层的当前词条
   }
   printf ( "\n" );
}
