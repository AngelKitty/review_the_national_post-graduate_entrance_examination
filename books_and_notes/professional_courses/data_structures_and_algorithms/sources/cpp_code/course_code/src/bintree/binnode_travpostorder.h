/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

/*DSA*/#include "../stack/Stack.h" //引入栈模板类
/*DSA*/#include "BinNode_TravPostorder_R.h"
/*DSA*/#include "BinNode_TravPostorder_I.h"
template <typename T> template <typename VST> //元素类型、操作器
void BinNode<T>::travPost ( VST& visit ) { //二叉树后序遍历算法统一入口
   switch ( rand() % 2 ) { //此处暂随机选择以做测试，共两种选择
      case 1: travPost_I ( this, visit ); break; //迭代版
      default: travPost_R ( this, visit ); break; //递归版
   }
}