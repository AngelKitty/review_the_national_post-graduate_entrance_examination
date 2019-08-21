/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * 基于邻接边表实现图的边结构
 */

package  dsa;

public class Edge_List implements Edge {
   //变量
   protected   Object info;//当前边中存放的数据元素
   protected Position ePosInE;//当前边在所属的图的边表中的位置
   protected Position vPosInV[];//当前边两个端点在顶点表中的位置
   protected Position ePosInI[];//当前边在其两个端点的关联边表中的位置
   //约定：第0（1）个顶点分别为尾（头）顶点
   //禁止头、尾顶点相同的边
   protected int type;//（经过遍历之后）边被归入的类别

   //构造方法：在图G中，生成一条从顶点u到v的新边（假定该边尚不存在）
   public Edge_List(Graph G, Vertex_List u, Vertex_List v, Object x) {
      info = x;//数据元素
      ePosInE = G.insert(this);//当前边在所属的图的边表中的位置
      vPosInV = new DLNode[2];//当前边两个端点在顶点表中的位置
      vPosInV[0] = u.getVPosInV(); vPosInV[1] = v.getVPosInV();
      ePosInI = new DLNode[2];//当前边在其两个端点的关联边表中的位置
      ePosInI[0] = u.outEdges.insertLast(this);//当前边加入u的邻接（出）边表
      ePosInI[1] = v.inEdges.insertLast(this);//当前边加入v的邻接（入）边表
      type = UNKNOWN;
   }

   //返回当前边的信息
   public Object getInfo() { return info; }
   //将当前边的信息更新为x，并返回原先的信息
   public Object setInfo(Object x) { Object e = info; info = x; return e; }

   //取当前边在所属的图的边集E中的位置
   public Position getEPosInE() { return ePosInE; }
   //取v[i]在顶点集V中的位置（i=0或1，分别对应于起点、终点）
   public Position getVPosInV(int i)   { return vPosInV[i]; }
   //当前边在其两个端点的关联边集I(v[i])中的位置
   public Position getEPosInI(int i) { return ePosInI[i]; }

   //读取、设置边的类别（针对遍历）
   public int getType() { return type; }
   public int setType(int t) { int tt = type; type = t; return tt; }

}
