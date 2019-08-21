void CreateLoserTree(LoserTree &ls) {  // 算法11.3
  // 已知b[0]到b[k-1]为完全二叉树ls的叶子结点存有k个关键字，
  // 沿从叶子到根的k条路径将ls调整成为败者树
  int i;
  b[k].key = MINKEY;                  // 设MINKEY为关键字可能的最小值
  for (i=0; i<k; ++i) ls[i] = k;      // 设置ls中"败者"的初值
  for (i=k-1; i>=0; --i) Adjust(ls, i);  
      // 依次从b[k-1],b[k-2],...,b[0]出发调整败者
} // CreateLoserTree
