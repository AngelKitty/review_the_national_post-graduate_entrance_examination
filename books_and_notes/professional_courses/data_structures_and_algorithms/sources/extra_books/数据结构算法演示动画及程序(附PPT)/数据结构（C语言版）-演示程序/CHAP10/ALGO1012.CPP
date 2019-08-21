void Merge (RedType SR[], RedType TR[], int i, int m, int n) {
   // 算法10.12
   // 将有序的SR[i..m]和SR[m+1..n]归并为有序的TR[i..n]
   int j,k;
   for (j=m+1, k=i;  i<=m && j<=n;  ++k) {   
      // 将SR中记录由小到大地并入TR
      if LQ(SR[i].key,SR[j].key) TR[k] = SR[i++];
      else TR[k] = SR[j++];
   }
   if (i<=m)  // TR[k..n] = SR[i..m];  将剩余的SR[i..m]复制到TR
      while (k<=n && i<=m) TR[k++]=SR[i++];
   if (j<=n)  // 将剩余的SR[j..n]复制到TR
      while (k<=n &&j <=n) TR[k++]=SR[j++];
} // Merge
