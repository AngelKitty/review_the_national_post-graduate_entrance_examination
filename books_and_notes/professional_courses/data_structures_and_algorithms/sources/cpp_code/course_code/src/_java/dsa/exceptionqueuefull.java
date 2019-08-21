/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 当试图对满队列应用enqueue方法时，本意外将被抛出
 */

package dsa;

public class ExceptionQueueFull extends RuntimeException {
   public ExceptionQueueFull(String err) {
      super(err);
   }
}
