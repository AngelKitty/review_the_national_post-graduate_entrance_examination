/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 基于邻接边表实现图结构
 */

package dsa;

public class Graph_List implements Graph {
   //变量
   protected List E;//容器：存放图中所有边
   protected List V;//容器：存放图中所有顶点

   //构造方法
   public Graph_List() { E = new List_DLNode(); V = new List_DLNode(); }

   //取图的边表、顶点表
   protected List getE() { return E; }
   protected List getV() { return V; }

   //取图中顶点、边的数目
   public int vNumber() { return V.getSize(); }
   public int eNumber() { return E.getSize(); }

   //取图中所有顶点、顶点位置的迭代器
   public Iterator vertices() { return V.elements(); }
   public Iterator vPositions() { return V.positions(); }

   //返回图中所有边、边位置的迭代器
   public Iterator edges() { return E.elements(); }
   public Iterator ePositions() { return E.positions(); }

   //检测是否有某条边从顶点u指向v
   public boolean areAdjacent(Vertex u, Vertex v)
   { return (null != edgeFromTo(u, v)); }
   //取从顶点u指向v的边，若不存在，则返回null
   public Edge edgeFromTo(Vertex u, Vertex v) {
      for (Iterator it = u.outEdges(); it.hasNext();) {//逐一检查
         Edge e = (Edge)it.getNext();//以u为尾的每一条边e
         if (v == e.getVPosInV(1).getElem())//若e是(u, v)，则
            return e;//返回该边
      }
      return null;//若不存在这样的(u, v)，则返回null
   }

   //将顶点v从顶点集中删除，并返回之
   public Vertex remove(Vertex v) {
      while (0 < v.outDeg())//将以v为尾的所有边
         remove((Edge)(((Vertex_List)v).outEdges.first()).getElem());//逐一删除
      while (0 < v.inDeg())//将以v为头的所有边
         remove((Edge)(((Vertex_List)v).inEdges.first()).getElem());//逐一删除
      return (Vertex)V.remove(v.getVPosInV());//在顶点表中删除v
   }
   //将边e从边集中删除，并返回之
   public Edge remove(Edge e) {
      ((Vertex_List)e.getVPosInV(0).getElem()).outEdges.remove(e.getEPosInI(0));//从起点的出边表中删除e
      ((Vertex_List)e.getVPosInV(1).getElem()).inEdges.remove(e.getEPosInI(1));//从终点的入边表中删除e
      return (Edge)E.remove(e.getEPosInE());//从边表中删除e
   }

   //在顶点集V中插入新顶点v，并返回其位置
   public Position insert(Vertex v) { return V.insertLast(v); }
   //在边集E中插入新边e，并返回其位置
   public Position insert(Edge e) { return E.insertLast(e); }
}
