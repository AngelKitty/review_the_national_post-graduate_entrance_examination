/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> //根据相对优先级确定适宜的方式，合并以a和b为根节点的两个左式堆
static BinNodePosi(T) merge ( BinNodePosi(T) a, BinNodePosi(T) b ) {
   if ( ! a ) return b; //退化情况
   if ( ! b ) return a; //退化情况
   if ( lt ( a->data, b->data ) ) swap ( a, b ); //一般情况：首先确保b不大
   a->rc = merge ( a->rc, b ); //将a的右子堆，与b合并
   a->rc->parent = a; //并更新父子关系
   if ( !a->lc || a->lc->npl < a->rc->npl ) //若有必要
      swap ( a->lc, a->rc ); //交换a的左、右子堆，以确保右子堆的npl不大
   a->npl = a->rc ? a->rc->npl + 1 : 1; //更新a的npl
   return a; //返回合并后的堆顶
} //本算法只实现结构上的合并，堆的规模须由上层调用者负责更新