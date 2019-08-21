void Rearrange(SqList &L, int adr[]) {// 算法 10.18
  // adr给出顺序表L的有序次序，即L.r[adr[i]]是第i小的记录。
  // 本算法按adr重排L.r，使其有序。
  int i,j,k;
  for (i=1; i<L.length; ++i)
    if (adr[i]!=i) {             // 第i小的记录未按序到位
      j = i;   L.r[0] = L.r[i];  // 暂存记录L.r[i]
      while (adr[j]!=i) { // 调整L.r[adr[j]]的记录到位直到adr[j]=i为止
        k = adr[j];   L.r[j] = L.r[k];
        adr[j] = j;   j = k;
      }
      L.r[j] = L.r[0];   adr[j] = j;  // 记录按序到位
    }
} // Rearrange
                             
