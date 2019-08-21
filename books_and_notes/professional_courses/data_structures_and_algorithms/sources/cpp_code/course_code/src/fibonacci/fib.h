/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

class Fib { //Fibonacci数列类
private:
   int f, g; //f = fib(k - 1), g = fib(k)。均为int型，很快就会数值溢出
public:
   Fib ( int n ) //初始化为不小于n的最小Fibonacci项
   { f = 1; g = 0; while ( g < n ) next(); } //fib(-1), fib(0)，O(log_phi(n))时间
   int get()  { return g; } //获取当前Fibonacci项，O(1)时间
   int next() { g += f; f = g - f; return g; } //转至下一Fibonacci项，O(1)时间
   int prev() { f = g - f; g -= f; return g; } //转至上一Fibonacci项，O(1)时间
};