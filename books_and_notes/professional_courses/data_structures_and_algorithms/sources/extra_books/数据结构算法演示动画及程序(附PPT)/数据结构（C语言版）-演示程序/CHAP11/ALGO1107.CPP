void Construct_Loser(LoserTree &ls, WorkArea &wa) {  // 算法11.7
  // 输入w个记录到内存工作区wa，建得败者树ls，
  // 选出关键字最小的记录并由s指示其在wa中的位置
  int i;
  for (i=0; i<w; ++i){
    wa[i].rnum = wa[i].key = ls[i] = 0;   // 工作区初始化
  }
  for (i=w-1; i>=0; --i) {
    fread(&wa[i].rec, sizeof(RcdType), 1, fi);  // 输入一个记录
    wa[i].key = wa[i].rec.key;    // 提取关键字
    wa[i].rnum = 1;               // 其段号为"1"
    Select_MiniMax (ls, wa, i);   // 算法11.6，调整败者 
  }
} // Construct_Loser
