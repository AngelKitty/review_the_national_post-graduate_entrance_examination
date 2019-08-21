int Index(SString S, SString T, int pos) {  // 算法4.5
   // 返回子串T在主串S中第pos个字符之后的位置。
   // 若不存在，则函数值为0。
   // 其中，T非空，1≤pos≤StrLength(S)。
   int i = pos;
   int j = 1;
   while (i <= S[0] && j <= T[0]) {
      if (S[i] == T[j]) {  // 继续比较后继字符
         ++i;
         ++j; 
      } else {  // 指针后退重新开始匹配
         i = i-j+2;
         j = 1; 
      }      
   }
   if (j > T[0]) return i-T[0];
   else return 0;
} // Index
