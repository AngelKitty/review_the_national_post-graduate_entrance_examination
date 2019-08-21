/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * ≤‚ ‘TreeMap
 */
import java.util.*;

public class TestTreeMap {
   public static void main(String[] args) {
      TreeMap scarborough = new TreeMap();
      scarborough.put("P", "parsley");
      scarborough.put("S", "sage");
      scarborough.put("R", "rosemary");
      scarborough.put("T", "thyme");
      System.out.println(scarborough); // {P = parsley, R = rosemary, S = sage, T = thyme}
   }
}
