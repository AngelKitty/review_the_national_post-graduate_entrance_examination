/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * （有向）图基于DFS模板的可达分量算法
 */

package dsa;

public class DFSReachableComponent extends DFS {
   //构造方法
   public DFSReachableComponent(Graph g) { super(g); }

   //DFS过程中对顶点v的具体访问操作（info实际上是一个栈，记录所有可达的顶点）
   protected Object visit(Vertex v, Object info)
   { ((Stack)info).push(v); return null; }

   //基于DFS的可达性算法：s为起始顶点（info实际上是一个栈，记录所有可达的顶点）
   public Object algorithm(Vertex s, Object info) {
      reset(s);
      Stack S = new Stack_Array();//保存从起始顶点可达的顶点
      traverse(s, info);//DFS
      return null;
   }
}
