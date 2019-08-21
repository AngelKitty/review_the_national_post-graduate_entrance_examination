/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * （有向）带权图的单源点最短路径算法
 */

package dsa;

public class BestFSDijkstra extends BestFS {
   //构造方法
   public BestFSDijkstra(Graph g) { super(g); }

   //更新尚未访问的顶点到源点的最短距离
   protected void updateDistanceAfter(Vertex v) {
      for (Iterator it = v.outEdges(); it.hasNext();) {//检查与顶点v
         Edge e = (Edge)it.getNext();//通过边e = (v, w)
         Vertex w = (Vertex)e.getVPosInV(1).getElem();//相联的每一顶点w
         int   weight = ((Integer)e.getInfo()).intValue();//根据边(v, w)的权重
         if (w.getDistance() > v.getDistance() + weight) {//取原距离与新距离中的小者
            w.setDistance(v.getDistance() + weight);
            w.setBFSParent(v);
         }
      }
   }
}
