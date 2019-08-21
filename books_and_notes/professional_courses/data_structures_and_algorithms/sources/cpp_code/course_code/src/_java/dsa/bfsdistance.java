/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * （有向）图基于BFS的最短距离算法
 */

package dsa;

public class BFSDistance extends BFS {
   //构造方法
   public BFSDistance(Graph g) { super(g); }

   //顶点访问操作：在本算法中，info是顶点v的前驱
   protected Object visit(Vertex v, Object info)   {
      if (null == info)//v为BFS的起始顶点
         v.setDistance(0);
      else
         v.setDistance(((Vertex)info).getDistance() + 1); //设置v到s的距离 = 前驱的距离+1
      return null;
   }

   //基于BFS实现的最短距离算法：s为起始顶点，info向算法传递参数
   public Object algorithm(Vertex s, Object info) {
      reset(s);
      traverse(s, info);//BFS：到起点的最短距离记录在各顶点的distance域中
      return null;
   }
}
