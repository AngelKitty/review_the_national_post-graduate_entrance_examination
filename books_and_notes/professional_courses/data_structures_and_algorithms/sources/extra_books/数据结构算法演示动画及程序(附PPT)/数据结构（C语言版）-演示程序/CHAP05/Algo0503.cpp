Status MultSMatrix(RLSMatrix M, RLSMatrix N, RLSMatrix &Q) {  
  // 算法5.3
  // 求矩阵乘积Q=M?N，采用行逻辑链接存储表示。
  int arow,brow,p,q,t,ctemp[30],l,ccol,tp;
  if (M.nu != N.mu) return ERROR;
  Q.mu = M.mu; Q.nu = N.nu; Q.tu = 0;       // Q初始化
  if (M.tu*N.tu != 0) {   // Q是非零矩阵
    for (arow=1; arow<=M.mu; ++arow) {      // 处理M的每一行
      for (l=1; l<=M.nu; ++l) ctemp[l] = 0; // 当前行各元素累加器清零
      Q.rpos[arow] = Q.tu+1; 
      if (arow<M.mu) tp=M.rpos[arow+1];
      else tp=M.tu+1;
      for (p=M.rpos[arow]; p<tp;++p) { // 对当前行中每一个非零元    
        brow=M.data[p].j;          // 找到对应元在N中的行号
        if (brow < N.mu ) t = N.rpos[brow+1];
        else t = N.tu+1;
        for (q=N.rpos[brow];  q< t;  ++q) {
          ccol = N.data[q].j;            // 乘积元素在Q中列号
          ctemp[ccol] += M.data[p].e * N.data[q].e;
        } // for q
      } // 求得Q中第crow( =arow)行的非零元
      for (ccol=1; ccol<=Q.nu; ++ccol) // 压缩存储该行非零元
        if (ctemp[ccol]) { 
          if (++Q.tu > MAXSIZE) return ERROR;
          Q.data[Q.tu].i=arow;
          Q.data[Q.tu].j=ccol;
          Q.data[Q.tu].e=ctemp[ccol];
        } // if
    } // for arow
  } // if   
  return OK;
} // MultSMatrix
