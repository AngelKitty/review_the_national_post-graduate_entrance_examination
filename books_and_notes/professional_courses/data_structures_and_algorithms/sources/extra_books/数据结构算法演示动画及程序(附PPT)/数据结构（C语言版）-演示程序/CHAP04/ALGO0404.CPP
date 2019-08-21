Status StrInsert(HString &S, int pos, HString T) {  // 算法4.4
   // 1≤pos≤StrLength(S)＋1。在串S的第pos个字符之前插入串T。
   int i;
   if (pos < 1 || pos > S.length+1)  // pos不合法
      return ERROR;  
   if (T.length) {    // T非空,则重新分配空间,插入T
      if (!(S.ch = (char *)realloc(S.ch,(S.length+T.length+1)
                                        *sizeof(char))))
         return ERROR;
      for (i=S.length-1; i>=pos-1; --i)  // 为插入T而腾出位置
         S.ch[i+T.length] = S.ch[i];
      for (i=0; i<T.length; i++)         // 插入T
         S.ch[pos-1+i] = T.ch[i]; 
      S.length += T.length;
   }
   return OK;
} // StrInsert
