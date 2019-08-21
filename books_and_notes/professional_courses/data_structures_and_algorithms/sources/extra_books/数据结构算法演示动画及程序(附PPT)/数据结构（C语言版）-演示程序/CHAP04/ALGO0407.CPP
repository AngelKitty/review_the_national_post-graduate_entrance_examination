void get_next(SString T, int *next) {  // 算法4.7
  int i=1;
  next[1]=0;
  int j=0;
  while (i<T[0]) {
    if(j==0 || T[i]== T[j]) {
      ++i;  ++j;  next[i] = j;
    } else j= next[j];
  }
}

int Index_KMP(SString S, SString T, int pos) {  // 算法4.6
  // 利用模式串T的next函数求T在主串S中第pos个字符之后的位置的
  // KMP算法。其中，T非空，1≤pos≤StrLength(S)。
  int next[255];
  int i = pos;
  int j = 1;
  get_next(T, next);
  while (i <= S[0] && j <= T[0]) {
    if (j == 0 || S[i] == T[j]) {  // 继续比较后继字符
      ++i;  ++j;
    } else j = next[j]; // 模式串向右移动
  }
  if (j > T[0]) return  i-T[0];   // 匹配成功
  else return 0;
} // Index_KMP
