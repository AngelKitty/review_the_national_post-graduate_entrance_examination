/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 测试向量
 */
import dsa.*;
import java.io.*;

public class TestVector {
   static final int TEST_SIZE = 31;
   public static void main(String[] args) {
      Vector vec = new Vector_ExtArray();
      //    Vector vec = new Vector_Array();
      while (TEST_SIZE > vec.getSize()) {
         double dice = Math.random();
         if (0.35 < dice)
            vec.insertAtRank((int)(Math.random()*(1 + vec.getSize())), new Integer((int)(Math.random()*(4 * TEST_SIZE))));
         else if (!vec.isEmpty())
            vec.removeAtRank((int)(Math.random()*vec.getSize()));
         Print(vec);
      }
   }

   //输出向量中的所有元素
   protected static void Print(Vector v) {
      for (int i = 0; i < v.getSize(); i++)
         System.out.print(v.getAtRank(i) + " ");
      System.out.println();
   }

}