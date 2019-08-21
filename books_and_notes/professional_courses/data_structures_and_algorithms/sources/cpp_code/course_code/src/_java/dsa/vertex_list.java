/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 基于邻接边表实现图的顶点结构
 */

package  dsa;

public class Vertex_List implements Vertex {
   //变量
   protected Object info;//当前顶点中存放的数据元素
   protected Position vPosInV;//当前顶点在所属的图的顶点表V中的位置
   protected List outEdges;//关联边表：存放以当前顶点为尾的所有边（的位置）
   protected List inEdges;//关联边表：存放以当前顶点为头的所有边（的位置）
   protected int status;//（在遍历图等操作过程中）顶点的状态
   protected int dStamp;//时间标签：DFS过程中该顶点被发现时的时刻
   protected int fStamp;//时间标签：DFS过程中该顶点被访问结束时的时刻
   protected int distance;//到指定起点的距离：BFS、Dijkstra等算法所确定该顶点到起点的距离
   protected Vertex bfsParent;//在最短距离树（BFS或BestFS）中的父亲

   //构造方法：在图G中引入一个属性为x的新顶点
   public Vertex_List(Graph G, Object x) {
      info = x;//数据元素
      vPosInV = G.insert(this);//当前顶点在所属的图的顶点表V中的位置
      outEdges = new List_DLNode();//出边表
      inEdges = new List_DLNode();//入边表
      status = UNDISCOVERED;
      dStamp = fStamp = Integer.MAX_VALUE;
      distance = Integer.MAX_VALUE;
      bfsParent = null;
   }

   //返回当前顶点的信息
   public Object getInfo() { return info; }
   //将当前顶点的信息更新为x，并返回原先的信息
   public Object setInfo(Object x) { Object e = info; info = x; return e; }

   //返回当前顶点的出、入度
   public int outDeg() { return outEdges.getSize(); }
   public int inDeg() { return inEdges.getSize(); }

   //返回当前顶点所有关联边、关联边位置的迭代器
   public Iterator inEdges() { return inEdges.elements(); }
   public Iterator inEdgePositions() { return inEdges.positions(); }
   public Iterator outEdges() { return outEdges.elements(); }
   public Iterator outEdgePositions() { return outEdges.positions(); }

   //取当前顶点在所属的图的顶点集V中的位置
   public Position getVPosInV() { return vPosInV; }

   //读取、设置顶点的状态（DFS + BFS）
   public int getStatus() { return status; }
   public int setStatus(int s) { int ss = status; status = s; return ss; }

   //读取、设置顶点的时间标签（DFS）
   public int getDStamp() { return dStamp; }
   public int setDStamp(int s) { int ss = dStamp; dStamp = s; return ss; }
   public int getFStamp() { return fStamp; }
   public int setFStamp(int s) { int ss = fStamp; fStamp = s; return ss; }

   //读取、设置顶点至起点的最短距离（BFS）
   public int getDistance() { return distance; }
   public int setDistance(int s) { int ss = distance; distance = s; return ss; }

   //读取、设置顶点在的DFS、BFS、BestFS或MST树中的父亲
   public Vertex getBFSParent() { return bfsParent; }
   public Vertex setBFSParent(Vertex s) { Vertex ss = bfsParent; bfsParent = s; return ss; }
}
