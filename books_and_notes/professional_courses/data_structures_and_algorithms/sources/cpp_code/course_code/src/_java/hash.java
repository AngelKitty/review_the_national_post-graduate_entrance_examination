/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * Example of HashMap and Hashtable in Java
 */
import java.util.*;

public class Hash {
   public static void main(String[] args) {
      HashMap HM = new HashMap();  //Map
      HM.put("东岳", "泰山");
      HM.put("西岳", "华山");
      HM.put("南岳", "衡山");
      HM.put("北岳", "恒山");
      HM.put("中岳", "嵩山");
      System.out.println(HM);
      Hashtable HT = new Hashtable();  //Dictionary
      HT.put("东岳", "泰山");
      HT.put("西岳", "华山");
      HT.put("南岳", "衡山");
      HT.put("北岳", "恒山");
      HT.put("中岳", "嵩山");
      System.out.println(HT);
   }
}
