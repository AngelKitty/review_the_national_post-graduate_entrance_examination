/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*
 * （有向）图的深度优先遍历算法模板
 */

package dsa;

public abstract class DFS extends GraphTraverse {
   //变量
   protected static int clock = 0;//遍历过程中使用的计时钟

   //构造方法
   public DFS(Graph g) { super(g); }

   //深度优先遍历算法
   protected Object traverse(Vertex v, Object info) {//从顶点v出发，做深度优先查找
      if (UNDISCOVERED != v.getStatus())  return null;//跳过已访问过的顶点（针对非连通图）
      v.setDStamp(clock++);      v.setStatus(DISCOVERED);   visit(v, info);//访问当前顶点
      for (Iterator it = v.outEdges(); it.hasNext();) {//检查与顶点v
         Edge e = (Edge)it.getNext();//通过边e = (v, u)
         Vertex u = (Vertex)e.getVPosInV(1).getElem();//相联的每一顶点u
         switch (u.getStatus()) {//根据u当前的不同状态，分别做相应处理
            case  UNDISCOVERED ://若u尚未被发现，则
               e.setType(TREE);//e被归类为“树边”
               traverse(u, info);//从u出发，继续做深度优先查找
               break;
            case  DISCOVERED ://若u已经被发现，但对其访问尚未结束，则
               e.setType(BACKWARD);//将e归类为“后向跨边”
               break;
            default ://VISITED，即对u的访问已经结束
               if (u.getDStamp() < v.getDStamp())//若相对于v，u被发现得更早，则
                  e.setType(CROSS);//将e归类为“横跨边”
               else//否则
                  e.setType(FORWARD);//将e归类为“前向跨边”
               break;
         }
      }//至此，v的所有邻居都已访问结束，故
      v.setFStamp(clock++);   v.setStatus(VISITED);//将v标记为VISITED
      return null;//然后回溯
   }
}
