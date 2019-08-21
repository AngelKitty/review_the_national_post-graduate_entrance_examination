Status MergeFile (FILE *f, FILE *g, FILE *h) {  // 算法12.1
  // 由按关键字递增有序的非空顺序文件f和g归并得到新文件h，三个文件均
  // 已打开，其中，f和g为只读文件，文件中各附加一个最大关键字记录，
  // 且g文件中对该记录的操作为插入。h为只写文件
  RcdType fr, gr;
  fread(&fr, sizeof(RcdType), 1, f);
  fread(&gr, sizeof(RcdType), 1, g);
  while (!feof(f) || !feof(g)) {
    if (fr.key < gr.key) {                   // 复制"旧"主文件中记录
      fwrite(&fr, sizeof(RcdType), 1, h);
      /*if (!feof(f))*/ fread(&fr, sizeof(RcdType), 1, f);
    } else if (gr.code=='D' && fr.key==gr.key) {  
      // 删除"旧"主文件中记录，即不复制
      /*if (!feof(f))*/ fread(&fr, sizeof(RcdType), 1, f);
      /*if (!feof(g))*/ fread(&gr, sizeof(RcdType), 1, g);
    } else if (gr.code=='I' && fr.key>gr.key) {   
      // 插入，函数P把gr加工为h的结构
      fwrite(P(gr), sizeof(RcdType), 1, h);
      /*if (!feof(g))*/ fread(&gr, sizeof(RcdType), 1, g);
    } else if (gr.code=='U' && fr.key==gr.key) {  
      // 更改"旧"主文件中记录
      fwrite(Q(fr, gr), sizeof(RcdType), 1, h);  
          // 函数Q将fr和gr归并成一个h结构的记录
      /*if (!feof(f))*/ fread(&fr, sizeof(RcdType), 1, f);
      /*if (!feof(g))*/ fread(&gr, sizeof(RcdType), 1, g);
    } else return ERROR;    // 其它均为出错情况
  }  // while 
  return OK;
} // MergeFile
