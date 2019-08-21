/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * （有向）图结构接口
 * 无向图也可以看成是有向图，为此，只需将每条无向边替换为对称的一对有向边
 */

package  dsa;

public interface Graph {
   //取图中顶点、边的数目
   public int vNumber();
   public int eNumber();

   //取图中所有顶点、顶点位置的迭代器
   public Iterator vertices();
   public Iterator vPositions();

   //返回图中所有边、边位置的迭代器
   public Iterator edges();
   public Iterator ePositions();

   //检测是否有某条边从顶点u指向v
   public boolean areAdjacent(Vertex u, Vertex v);
   //取从顶点u指向v的边，若不存在，则返回null
   public Edge edgeFromTo(Vertex u, Vertex v);

   //将顶点v从顶点集中删除，并返回之
   public Vertex remove(Vertex v);
   //将边e从边集中删除，并返回之
   public Edge remove(Edge e);
   //在顶点集V中插入新顶点v，并返回其位置
   public Position insert(Vertex v);
   //在边集E中插入新边e，并返回其位置
   public Position insert(Edge e);
}