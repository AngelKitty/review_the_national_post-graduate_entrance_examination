/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

#include "QuadlistNode.h" //引入Quadlist节点类
template <typename T> class Quadlist { //Quadlist模板类
private:
   int _size; QlistNodePosi(T) header; QlistNodePosi(T) trailer; //规模、头哨兵、尾哨兵
protected:
   void init(); //Quadlist创建时的初始化
   int clear(); //清除所有节点
public:
// 构造函数
   Quadlist() { init(); } //默认
// 析构函数
   ~Quadlist() { clear(); delete header; delete trailer; } //删除所有节点，释放哨兵
// 只读访问接口
   Rank size() const { return _size; } //规模
   bool empty() const { return _size <= 0; } //判空
   QlistNodePosi(T) first() const { return header->succ; } //首节点位置
   QlistNodePosi(T) last() const { return trailer->pred; } //末节点位置
   bool valid ( QlistNodePosi(T) p ) //判断位置p是否对外合法
   { return p && ( trailer != p ) && ( header != p ); }
// 可写访问接口
   T remove ( QlistNodePosi(T) p ); //删除（合法）位置p处的节点，返回被删除节点的数值
   QlistNodePosi(T) //将*e作为p的后继、b的上邻插入
   insertAfterAbove ( T const& e, QlistNodePosi(T) p, QlistNodePosi(T) b = NULL );
// 遍历
   void traverse ( void (* ) ( T& ) ); //遍历各节点，依次实施指定操作（函数指针，只读或局部修改）
   template <typename VST> //操作器
   void traverse ( VST& ); //遍历各节点，依次实施指定操作（函数对象，可全局性修改节点）
}; //Quadlist

#include "Quadlist_implementation.h"