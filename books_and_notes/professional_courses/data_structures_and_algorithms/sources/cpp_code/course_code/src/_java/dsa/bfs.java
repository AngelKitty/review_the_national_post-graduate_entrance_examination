/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * （有向）图的广度优先遍历算法模板
 */

package dsa;

public abstract class BFS extends GraphTraverse {
   //构造方法
   public BFS(Graph g) { super(g); }

   //广度优先遍历算法
   protected Object traverse(Vertex s, Object info) {//从顶点s出发，做广度优先查找
      if (UNDISCOVERED != s.getStatus())  return null;//跳过已访问过的顶点（针对非连通图）
      Queue Q = new Queue_List();//借用一个队列
      s.setStatus(DISCOVERED);//发现s后
      Q.enqueue(s);//随即令其入队
      visit(s, null);//并访问之
      while (!Q.isEmpty()) {//在队列变空之前
         Vertex v = (Vertex)Q.dequeue();//取出队首顶点v
         for (Iterator it = v.outEdges(); it.hasNext();) {//检查与顶点v
            Edge e = (Edge)it.getNext();//通过边e = (v, u)
            Vertex u = (Vertex)e.getVPosInV(1).getElem();//相联的每一顶点u
            if (UNDISCOVERED == u.getStatus()) {//若u尚未被发现，则
               e.setType(TREE);//将e归类为树边
               u.setStatus(DISCOVERED);//发现u后
               Q.enqueue(u);//随即令其入队
               visit(u, v);//并访问之
            } else {//若u已被发现，甚至已访问完毕（有向图），则
               e.setType(CROSS);//将e归类为跨边
            }
         }//至此，v的所有邻居都已访问结束，故
         v.setStatus(VISITED);//将v标记为VISITED
      }//while
      return null;
   }
}
