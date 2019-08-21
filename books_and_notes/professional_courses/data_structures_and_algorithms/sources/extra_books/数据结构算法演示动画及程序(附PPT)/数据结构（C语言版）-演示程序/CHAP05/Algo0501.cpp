Status TransposeSMatrix(TSMatrix M, TSMatrix &T) {  // 算法5.1
  // 采用三元组顺序表存储表示，求稀疏矩阵M的转置矩阵T
  int p, q, col;
  T.mu = M.nu;  T.nu = M.mu;  T.tu = M.tu;
  if (T.tu) {
    q = 1;
    for (col=1; col<=M.nu; ++col)
      for (p=1; p<=M.tu; ++p)
        if (M.data[p].j == col) {
          T.data[q].i=M.data[p].j;T.data[q].j =M.data[p].i;
          T.data[q].e =M.data[p].e;  ++q; 
        }
  }
  return OK;
} // TransposeSMatrix
                              


