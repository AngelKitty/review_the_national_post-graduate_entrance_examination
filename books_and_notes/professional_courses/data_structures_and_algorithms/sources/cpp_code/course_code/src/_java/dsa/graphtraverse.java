/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * （有向）图的遍历算法模板
 */

package dsa;

public abstract class GraphTraverse {
   //常量
   final static int UNDISCOVERED = 0;//尚未被发现的顶点
   final static int DISCOVERED = 1;//已被发现的顶点
   final static int VISITED = 2;//已访问过的顶点
   final static int UNKNOWN = 0;//未知边
   final static int TREE = 1;//树边
   final static int CROSS = 2;//横跨边
   final static int FORWARD = 3;//前向跨边
   final static int BACKWARD = 4;//后向跨边

   //变量
   protected Graph G;//图

   //构造方法
   public GraphTraverse(Graph g) { G = g; }

   //将G中各顶点的标志、各边的分类复位（s为遍历起点）
   protected void reset(Vertex s) {
      for (Iterator it = G.vertices(); it.hasNext();) {//所有
         Vertex v = (Vertex)it.getNext();//顶点的
         v.setStatus(UNDISCOVERED);//状态置为UNDISCOVERED
         v.setDistance(Integer.MAX_VALUE);//最短距离初始化为无穷大
      }
      for (Iterator it = G.edges(); it.hasNext();)//所有边
         ((Edge)it.getNext()).setType(UNKNOWN);//置为UNKNOWN
   }

   //遍历过程中对顶点v的具体访问操作的模板：取决于、服务于具体的算法algorithm()
   protected abstract Object visit(Vertex v, Object info);

   //基于遍历操作实现的其它算法的模板：s为起始顶点，info向算法传递参数及保存算法的返回信息
   public abstract Object algorithm(Vertex s, Object info);

   //遍历算法模板
   protected abstract Object traverse(Vertex v, Object info);//从顶点v出发做遍历
}
