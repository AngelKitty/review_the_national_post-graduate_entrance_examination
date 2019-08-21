/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

#include "../Dictionary/Dictionary.h" //引入词典ADT
#include "../Bitmap/Bitmap.h" //引入位图

template <typename K, typename V> //key、value
class Hashtable : public Dictionary<K, V> { //符合Dictionary接口的Hashtable模板类
   /*DSA*/friend class UniPrint;
private:
   Entry<K, V>** ht; //桶数组，存放词条指针
   int M; //桶数组容量
   int N; //词条数量
   Bitmap* lazyRemoval; //懒惰删除标记
#define lazilyRemoved(x)  (lazyRemoval->test(x))
#define markAsRemoved(x)  (lazyRemoval->set(x))
protected:
   int probe4Hit ( const K& k ); //沿关键码k对应的查找链，找到词条匹配的桶
   int probe4Free ( const K& k ); //沿关键码k对应的查找链，找到首个可用空桶
   void rehash(); //重散列算法：扩充桶数组，保证装填因子在警戒线以下
public:
   Hashtable ( int c = 5 ); //创建一个容量不小于c的散列表（为测试暂时选用较小的默认值）
   ~Hashtable(); //释放桶数组及其中各（非空）元素所指向的词条
   int size() const { return N; } // 当前的词条数目
   bool put ( K, V ); //插入（禁止雷同词条，故可能失败）
   V* get ( K k ); //读取
   bool remove ( K k ); //删除
};

#include "Hashtable_implementation.h"
