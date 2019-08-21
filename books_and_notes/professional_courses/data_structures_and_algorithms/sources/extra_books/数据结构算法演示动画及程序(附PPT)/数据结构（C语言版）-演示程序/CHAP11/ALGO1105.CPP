void get_run(LoserTree &ls, WorkArea &wa) {   // 算法11.5
  // 求得一个初始归并段，fi为输入文件指针，fo为输出文件指针
  while (wa[ls[0]].rnum == rc) {    // 选得的MINIMAX记录属当前段时
    int q = ls[0];                  // q指示MINIMAX记录在wa中的位置
    minimax = wa[q].key;
    fwrite(&(wa[q].rec), sizeof(RcdType), 1, fo); 
        // 将刚选的MINIMAX记录写入输出文件
    if (fread(&wa[q].rec, sizeof(RcdType), 1, fi)==1) { 
      // 从输入文件读下一记录
      wa[q].key = wa[q].rec.key;        // 提取关键字
      if (wa[q].key < minimax) {        // 新读入的记录属下一段
        rmax = rc + 1;   wa[q].rnum = rmax;
      } else wa[q].rnum = rc;           // 新读入的记录属当前段
    } else // 当文件读取不成功或输入文件结束时，虚设记录(属"rmax+1"段)
      { wa[q].rnum = rmax+1;  wa[q].key = MAXKEY; }
    Select_MiniMax (ls, wa, q);         // 选择新的MINIMAX记录
  }
} // get_run
