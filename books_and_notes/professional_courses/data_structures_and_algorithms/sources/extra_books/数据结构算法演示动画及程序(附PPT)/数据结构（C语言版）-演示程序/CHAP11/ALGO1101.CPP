void K_Merge (LoserTree &ls, External &b) {  // 算法11.1
  // 利用败者树ls将编号从0到k-1的k个输入归并段中的记录归并到输出归并段。
  // b[0]至b[k-1]为败者树上的k个叶子结点，分别存放k个输入归并段中当前记
  // 录的关键字
  int i, q;
  for (i=0; i<k; ++i) input(b[i].key, i); //分别从k个输入归并段读入该段
                                      // 当前第一个记录的关键字到外结点
  CreateLoserTree(ls);   // 建败者树ls，选得最小关键字为b[ls[0]].key
  while (b[ls[0]].key != MAXKEY) {
    q = ls[0];   // q指示当前最小关键字所在输入归并段
    output(q);   // 将编号为q的输入归并段中当前(关键字为b[q].key)的
                 // 记录写至输出归并段
    input(b[q].key, q); // 从编号为q的输入归并段读入下一个记录的关键字
    Adjust(ls, q);      // 调整败者树，选择新的最小关键字
  }
  output(ls[0]);        // 将含最大关键字MAXKEY的记录写至输出归并段
} // K_Merge
