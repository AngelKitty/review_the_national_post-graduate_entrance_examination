void Select_MiniMax (LoserTree &ls, WorkArea wa, int q) { // 算法11.6
  //从wa[q]起到败者树的根比较选择MINIMAX记录，并由q指示它所在的归并段
  int t, p, temp;
  for (t=(w+q)/2, p=ls[t];  t>0;  t/=2, p=ls[t])
    if (wa[p].rnum<wa[q].rnum ||
        (wa[p].rnum==wa[q].rnum && wa[p].key<wa[q].key)) {
      temp=q;  q=ls[t];  ls[t]=temp;   // q指示新的胜利者
    }
  ls[0] = q;
} // Select_MiniMax
