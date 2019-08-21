/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

/*DSA*/#include "../stack/Stack.h" //引入栈模板类
/*DSA*/#include "BinNode_TravInorder_R.h"
/*DSA*/#include "BinNode_TravInorder_I1.h"
/*DSA*/#include "BinNode_TravInorder_I2.h"
/*DSA*/#include "BinNode_TravInorder_I3.h"
/*DSA*/#include "BinNode_TravInorder_I4.h"
template <typename T> template <typename VST> //元素类型、操作器
void BinNode<T>::travIn ( VST& visit ) { //二叉树中序遍历算法统一入口
   switch ( rand() % 5 ) { //此处暂随机选择以做测试，共五种选择
      case 1: travIn_I1 ( this, visit ); break; //迭代版#1
      case 2: travIn_I2 ( this, visit ); break; //迭代版#2
      case 3: travIn_I3 ( this, visit ); break; //迭代版#3
      case 4: travIn_I4 ( this, visit ); break; //迭代版#4
      default: travIn_R ( this, visit ); break; //递归版
   }
}