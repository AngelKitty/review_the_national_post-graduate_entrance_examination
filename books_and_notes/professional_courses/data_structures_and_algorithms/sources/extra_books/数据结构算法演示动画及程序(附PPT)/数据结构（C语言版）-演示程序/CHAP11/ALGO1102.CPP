void Adjust(LoserTree &ls, int s) {  // 算法11.2
  // 沿从叶子结点b[s]到根结点ls[0]的路径调整败者树。
  int t, temp;
  t = (s+k)/2;   // ls[t]是b[s]的双亲结点
  while (t>0) {
    if (b[s].key > b[ls[t]].key) {
      temp = s;  s = ls[t];  ls[t] = temp;   // s指示新的胜者
    }
    t /= 2;
  }
  ls[0] = s;
} // Adjust
