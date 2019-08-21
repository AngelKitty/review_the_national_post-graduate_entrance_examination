void MSort(RedType SR[], RedType TR1[], int s, int t) { // 算法10.13
   // 将SR[s..t]归并排序为TR1[s..t]。
   int m;
   RedType TR2[20];
   if (s==t) TR1[t] = SR[s];
   else {
      m=(s+t)/2;            // 将SR[s..t]平分为SR[s..m]和SR[m+1..t]
      MSort(SR,TR2,s,m);    // 递归地将SR[s..m]归并为有序的TR2[s..m]
      MSort(SR,TR2,m+1,t);  // 将SR[m+1..t]归并为有序的TR2[m+1..t]
      Merge(TR2,TR1,s,m,t); // 将TR2[s..m]和TR2[m+1..t]归并到TR1[s..t]
   }
} // MSort
