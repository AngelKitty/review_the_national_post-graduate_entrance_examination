/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 当试图对空队列应用front或dequeue方法时，本意外将被抛出
 */

package dsa;

public class ExceptionQueueEmpty extends RuntimeException {
   public ExceptionQueueEmpty(String err) {
      super(err);
   }
}
