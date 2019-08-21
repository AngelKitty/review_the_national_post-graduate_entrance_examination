void Replace_Selection(LoserTree &ls, WorkArea &wa, 
                       FILE *fi, FILE *fo) { // 算法11.4
  // 在败者树ls和内存工作区wa上用置换-选择排序求初始归并段，fi为输入文
  // 件（只读文件）指针，fo为输出文件（只写文件）指针，两文件均已打开
  RcdType RUNEND_SYMBOL;      //  段结束标志
  RUNEND_SYMBOL.key=MAXKEY;
  Construct_Loser (ls, wa);   // 初建败者树
  rc = rmax = 1;         // rc指示当前生成的初始归并段的段号，
                         // rmax指示wa中关键字所属初始归并段的最大段号
  while (rc <= rmax) { // "rc=rmax+1"标志输入文件的置换-选择排序已完成
    get_run(ls, wa);   // 求得一个初始归并段
    fwrite(&RUNEND_SYMBOL, sizeof(RcdType), 1, fo);  
        // 将段结束标志写入输出文件
    rc = wa[ls[0]].rnum;      // 设置下一段的段号
  }
} // Replace_Selection
