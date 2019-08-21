/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * （有向）图的边结构接口
 * 无向图也可以看成是有向图，为此，只需将每条无向边替换为对称的一对有向边
 */

package  dsa;

public interface Edge {
   //常量
   final static int UNKNOWN = 0;//未知边
   final static int TREE = 1;//树边
   final static int CROSS = 2;//横跨边
   final static int FORWARD = 3;//前向跨边
   final static int BACKWARD = 4;//后向跨边

   //返回当前边的信息（对于带权图，也就是各边的权重）
   public Object getInfo();
   //将当前边的信息更新为x，并返回原先的信息
   public Object setInfo(Object x);

   //取当前边在所属的图的边集E中的位置
   public Position getEPosInE();
   //取v[i]在顶点集V中的位置（i=0或1，分别对应于起点、终点）
   public Position getVPosInV(int i);
   //当前边在其两个端点的关联边集I(v[i])中的位置
   public Position getEPosInI(int i);

   //读取、设置边的类别（针对遍历）
   public int getType();
   public int setType(int t);
}
