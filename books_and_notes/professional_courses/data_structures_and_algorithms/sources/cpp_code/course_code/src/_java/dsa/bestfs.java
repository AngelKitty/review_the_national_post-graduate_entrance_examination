/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * （有向）带权图最佳优先遍历
 */

package dsa;

public abstract class BestFS extends GraphTraverse {
   //构造方法
   public BestFS(Graph g) { super(g); }

   //更新尚未访问的顶点到已访问点集的最短距离（取决于具体的算法）
   protected abstract void updateDistanceAfter(Vertex v);

   //最佳优先遍历算法
   protected Object traverse(Vertex s, Object info) {//从顶点s出发，做最佳优先遍历
      if (UNDISCOVERED != s.getStatus())  return null;//跳过已访问过的顶点（针对非连通图）
      s.setDistance(0);//设置s到已访问点集的距离
      Vertex v;//最佳顶点
      while (null != (v = bestVertex())) {//只要还有最佳顶点
         visit(v, null);//在发现并访问v之后
         updateDistanceAfter(v);//更新尚未访问的顶点到已访问集的最短距离
      }//while
      return null;
   }

   //顶点访问操作：在本算法中，info无用
   protected Object visit(Vertex v, Object info)   {
      v.setStatus(VISITED);//设置“已访问”标记
      return null;
   }

   //基于BestFS实现的最短距离算法：s为起始顶点，info向算法传递参数
   public Object algorithm(Vertex s, Object info) {
      reset(s);//初始化，标记复位
      traverse(s, info);//BestFS：到起点的最短距离记录在各顶点的distance域中
      return null;
   }

   //从尚未访问的顶点中选出最佳者
   //对于Dijkstra算法而言，就是与已访问集连通、距离最近的顶点（及距离不是无穷的最近顶点）
   //若没有这样的顶点，则返回null
   protected Vertex bestVertex() {
      int bestValue = Integer.MAX_VALUE;//最佳指标（越小越好）
      Vertex bestVertex = null;//最佳顶点
      for (Iterator it = G.vertices(); it.hasNext();) {//逐个检查
         Vertex u = (Vertex)it.getNext();//各个
         if (UNDISCOVERED == u.getStatus())//尚未访问的顶点u
            if (bestValue > u.getDistance())//若u到已访问点集距离更近，则
               { bestValue = u.getDistance();  bestVertex = u; }//更新最佳记录
      }
      if ((null != bestVertex) && (null != bestVertex.getBFSParent()))//最佳顶点与其父亲之间的联边
         G.edgeFromTo(bestVertex.getBFSParent(), bestVertex).setType(TREE);//被标记为TREE
      return bestVertex;
   }
}
