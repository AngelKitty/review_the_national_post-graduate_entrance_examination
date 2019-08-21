#include "algo0403.cpp"
int Index(SString S, SString T, int pos) {  // 算法4.1
   // T为非空串。若主串S中第pos个字符之后存在与T相等的子串，
   // 则返回第一个这样的子串在S中的位置，否则返回0
   int n,m,i;
   SString sub;
   if (pos > 0) {
      n = StrLength(S);  
      m = StrLength(T);
      i = pos;
      while (i <= n-m+1) {
         SubString (sub, S, i, m);
         if (StrCompare(sub,T) == 0) ++i;
         else return i;
      } // while
   } // if
   return 0;                    
}
