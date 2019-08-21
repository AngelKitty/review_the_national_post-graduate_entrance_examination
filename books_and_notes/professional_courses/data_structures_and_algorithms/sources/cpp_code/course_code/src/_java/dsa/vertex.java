/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * （有向）图的顶点结构接口
 * 无向图也可以看成是有向图，为此，只需将每条无向边替换为对称的一对有向边
 */

package  dsa;

public interface Vertex {
   //常量
   final static int UNDISCOVERED = 0;//尚未被发现的顶点
   final static int DISCOVERED = 1;//已被发现的顶点
   final static int VISITED = 2;//已访问过的顶点

   //返回当前顶点的信息
   public Object getInfo();
   //将当前顶点的信息更新为x，并返回原先的信息
   public Object setInfo(Object x);

   //返回当前顶点的出、入度
   public int outDeg();
   public int inDeg();

   //返回当前顶点所有关联边、关联边位置的迭代器
   public Iterator inEdges();
   public Iterator inEdgePositions();
   public Iterator outEdges();
   public Iterator outEdgePositions();

   //取当前顶点在所属的图的顶点集V中的位置
   public Position getVPosInV();

   //读取、设置顶点的状态（DFS + BFS）
   public int getStatus();
   public int setStatus(int s);

   //读取、设置顶点的时间标签（DFS）
   public int getDStamp();
   public int setDStamp(int s);
   public int getFStamp();
   public int setFStamp(int s);

   //读取、设置顶点至起点的最短距离（BFS或BestFS）
   public int getDistance();
   public int setDistance(int s);

   //读取、设置顶点在的DFS、BFS、BestFS或MST树中的父亲
   public Vertex getBFSParent();
   public Vertex setBFSParent(Vertex s);
}
