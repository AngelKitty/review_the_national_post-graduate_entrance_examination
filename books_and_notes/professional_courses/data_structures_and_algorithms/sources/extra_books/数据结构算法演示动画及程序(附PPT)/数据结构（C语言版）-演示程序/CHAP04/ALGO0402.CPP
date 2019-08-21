Status Concat(SString &T, SString S1, SString S2) { // 算法4.2
   // 用T返回由S1和S2联接而成的新串。若未截断，则返回TRUE，否则FALSE。
   int i;
   Status uncut;
   if (S1[0]+S2[0] <= MAXSTRLEN) {  // 未截断
      for (i=1; i<=S1[0]; i++) T[i] = S1[i];
      for (i=1; i<=S2[0]; i++) T[i+S1[0]] = S2[i];
      T[0] = S1[0]+S2[0];
      uncut = TRUE;
   } else if (S1[0] < MAXSTRLEN) {  // 截断
      for (i=1; i<=S1[0]; i++) T[i] = S1[i];
      for (i=S1[0]+1; i<=MAXSTRLEN; i++) T[i] = S2[i-S1[0]];
      T[0] = MAXSTRLEN;
      uncut = FALSE;
   } else {                         // 截断(仅取S1)
      for (i=0; i<=MAXSTRLEN; i++) T[i] = S1[i];
      uncut = FALSE;
   }
   return uncut;
} // Concat
