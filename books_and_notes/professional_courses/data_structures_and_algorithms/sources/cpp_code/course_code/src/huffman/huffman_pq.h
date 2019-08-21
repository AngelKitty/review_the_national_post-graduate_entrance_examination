/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/******************************************************************************************
 * 支持Huffman编码的数据结构
 ******************************************************************************************
 * 采用统一的接口，为Huffman_PQ_ComplHeap和Huffman_PQ_LeftHeap共用
 * 编译之前，可通过在对应的工程中设置DSA_PQ_ComplHeap或DSA_PQ_LeftHeap选择
 ******************************************************************************************/
#pragma once

#include <iostream>
using namespace std;

/*DSA*/#include "../UniPrint/print.h"

//Huffman树
#include "../Huffman/HuffChar.h" //引入Huffman（超）字符
#include "../Huffman/HuffTree.h" //引入Huffman树
#include "../Huffman/HuffTable.h" //引入Huffman编码表
#include "../Huffman/HuffCode.h" //引入HuffCode二进制编码

// Huffman森林
#if defined(DSA_PQ_LIST)
#include "../Huffman/Huffman_Forest_PQ_List.h"
#elif defined(DSA_PQ_COMPLHEAP)
#include "../Huffman/Huffman_Forest_PQ_ComplHeap.h"
#elif defined(DSA_PQ_LEFTHEAP)
#include "../Huffman/Huffman_Forest_PQ_LeftHeap.h"
#else //朴素方案：直接用List实现的Huffman森林
#include "../Huffman/Huffman_Forest_List.h"
#endif