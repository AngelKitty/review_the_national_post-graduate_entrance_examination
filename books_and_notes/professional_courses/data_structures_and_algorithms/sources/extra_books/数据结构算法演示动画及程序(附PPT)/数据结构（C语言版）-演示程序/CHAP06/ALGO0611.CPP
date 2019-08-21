int fix_mfset(MFSet &S, int i) {  // 算法6.11
   // 确定i所在子集，并将从i至根路径上所有结点都变成根的孩子结点。
   int j,k,t;
   if (i<1 || i>S.n) return -1;       // i 不是S中任一子集的成员
   for (j=i; S.nodes[j].parent>=0; j=S.nodes[j].parent) 
      printf("\t%d(%d%3d)\n", j, S.nodes[j].data, S.nodes[j].parent);
   for (k=i; k!=j; k=t) {
      t=S.nodes[k].parent;  S.nodes[k].parent=j;
   }
   return 1;
} // fix_mfset
                               
