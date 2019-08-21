#define  MaxBookNum      1000    // 假设只对1000本书建索引表
#define  MaxKeyNum       2500    // 索引表的最大容量
#define  MaxLineLen       500    // 书目串的最大长度
#define  MaxWordNum        10    // 词表的最大容量
#define  MaxWordLength     30    // 单词的最大长度

typedef int Boolean;

typedef struct {
    char    item[MaxWordNum][MaxWordLength];  // 字符串的数组
    int     last;        // 词表的长度
} WordListType;          // 词表类型（顺序表）

typedef struct {
    HString   key;       // 关键词
    NLinkList bnolist;   // 存放书号索引的链表
} IdxTermType;           // 索引项类型

typedef struct {
    IdxTermType  item[MaxKeyNum+1];
    int          last;
} IdxListType;           // 索引表类型（有序表）

//------ 基本操作 ------
void InitIdxList(IdxListType &idxlist);
   // 初始化操作，置索引表idxlist为空表，即idxlist.last=0,
   // 且在idxlist.item[0]设一空串
void GetLine(FILE *f);
   // 从文件f读入一个书目信息到书目串缓冲区buf
Status ExtractKeyWord(char *Buffer, WordListType &w, int &bno);
   // 从buf中提取书名关键词到词表wdlist，书号存入bno
Status InsertIdxList(IdxListType &idxlist, ElemType bno);  
   // 将书号为bno的书名关键词按词典顺序插入索引表idxlist
Status PutText(FILE *g, IdxListType idxlist);           
   // 将生成的索引表idxlist输出到文件g
void PrintFile(FILE *FileName);

Status InsertIdxList(IdxListType &idxlist, int bno);   // 算法4.10
   // 索引表插入算法
void GetWord(int i, HString &wd);                      // 算法4.11
   // 用wd返回词表wdlist中第i个关键词。
int Locate(IdxListType &idxlist, HString wd, Boolean &b); // 算法4.12
   // 在索引表idxlist中查询是否存在与wd相等的关键词。
   // 若存在，则返回其在索引表中的位置，
   // 且b取值TRUE；否则返回插入位置，且b取值FALSE
void InsertNewKey(IdxListType &idxlist, int i, HString wd);//算法4.13
   // 在索引表idxlist的第i项上插入新关键词wd，
   // 并初始化书号索引的链表为空表
Status InsertBook(IdxListType &idxlist, int i, int bno); // 算法4.14
   // 在索引表idxlist的第i项中插入书号为bno的索引

//------ 主要变量 ------
char          buf[MaxLineLen];   // 书目串缓冲区
WordListType  wdlist;    // 词表
IdxListType   idxlist;   // 索引表

//------ 主函数 ------
int main(int argc, char* argv[]) {  // 算法4.9
  FILE *f,*g;
  int BookNo;
  printf("**************************************************\n");
  printf("       《数据结构》(C语言版) 严蔚敏 吴伟民 编著     \n");
  printf("                算法4.9-4.14（参见图4.9）          \n");
  printf("**************************************************\n\n");
  if ((f = fopen ("Algo0409BookInfo.txt", "r"))==NULL) {
    printf("ERROR in open BookInfo.txt!\n");
    exit(1);
  }
  if ((g = fopen ("Algo0409BookIdx.txt", "w"))==NULL) {
    printf("ERROR in open BookIdx.txt!\n");
    exit(1);
  }
  printf("书目文件：\n");
  PrintFile(f);
  InitIdxList(idxlist);         // 初始化索引表idxlist为空表
  while (!feof (f)) {
    GetLine (f);                // 从文件f读入一个书目信息到buf
    ExtractKeyWord(buf,wdlist,BookNo);  
        // 从buf提取关键词到词表，书号存入BookNo
    InsertIdxList(idxlist, BookNo);  // 书号为BookNo的关键词插入索引表
  }
  PutText(g, idxlist);          // 将生成的索引表idxlist输出到文件g
  fclose(f);
  fclose(g);
  printf("对书目文件进行处理后的索引文件：\n");
  if ((g = fopen ("Algo0409BookIdx.txt", "r"))==NULL) {
    printf("ERROR in open BookIdx.txt!\n");
    exit(1);
  }
  PrintFile(g);
  fclose(g);
  printf("按任意键，结束 ......\n");
  getch();
  return 0;
} // main

Status InsertIdxList(IdxListType &idxlist,  int bno) {  // 算法4.10
  int i,j;
  HString wd;
  Boolean b;
  for (i=0;  i<wdlist.last;  i++) {
    GetWord(i, wd);   
    j = Locate(idxlist, wd, b);
    if (!b)
      InsertNewKey(idxlist, j, wd);  //  插入新的索引项
    InsertBook(idxlist, j, bno);     //  插入书号索引
  }
  return OK;
} // InsertIdxList

void GetWord(int i,  HString &wd) {  // 算法4.11
  char *p;
  p = *(wdlist.item +i);  // 取词表中第i个字符串
  StrAssign(wd, p);       // 生成关键字字符串
} // GetWord

int Locate(IdxListType &idxlist, HString wd, Boolean &b) { // 算法4.12
  int i,m;
  for (i = idxlist.last-1; 
       ((m = StrCompare(idxlist.item[i].key, wd)) > 0);  --i);
  if (m==0) { // 找到
    b = TRUE;
    return i;  
  } else {  // 没找到
    b = FALSE;
    return i+1;
  }         
} // Locate

void InsertNewKey(IdxListType &idxlist, int i, HString wd) {//算法4.13
  int j;
  for (j=idxlist.last-1;  j>=i;  --j)  // 后移索引项
    idxlist.item[j+1] = idxlist.item[j];
  // 插入新的索引项
  StrCopy(idxlist.item[i].key, wd);    // 串赋值
  InitList(idxlist.item[i].bnolist);   // 初始化书号索引表为空表
  ++idxlist.last;
} // InsertNewKey

Status InsertBook(IdxListType &idxlist, int i, int bno) {  //算法4.14
  NLink p;
  if (!MakeNode (p, bno)) 
    return OVERFLOW;                   // 分配失败
  Append(idxlist.item[i].bnolist, p);  // 插入新的书号索引
  return OK;
} // InsertBook

//------ 基本操作 -------
void InitIdxList(IdxListType &idxlist) {
  int i;
  idxlist.last= 0;
  for(i=0; i<MaxKeyNum+1; i++)
    InitList(idxlist.item[i].bnolist);
}

Status ExtractKeyWord(char* Buffer,WordListType &w,int &Num) {
  int i=0, j=0, k=0;
  bool Ignore;
  char TempChar[30];
  char IgnoreChar[7][10] = { "to","of","the","and","not","or","if" };
  w.last=0;
  while(*(Buffer+i)!= ' ') { TempChar[i]=*(Buffer+i);  i++; }
  i++;
  TempChar[i]= '\0';
  Num=atoi(TempChar);
  while(*(Buffer+i)!='\n' && *(Buffer+i)!='\0') { 
    // 每个字符串末尾都有作为结束符'\n'
    if(*(Buffer+i)!=' ') { // 若非空字符，则把当前字符加入新的字符串中
      if(*(Buffer+i)>='A' && *(Buffer+i)<='Z')  // 大写字母转换为小写
        *(Buffer+i)-='A'-'a';
      w.item[j][k]=*(Buffer+i);
      k++;  i++;
    } else {               // 如果是空字符，这是则开始另一个字符串
      Ignore=false;
      w.item[j][k++]='\0';
      for (int m=0; m<7; m++)
        if(strcmp(w.item[j],IgnoreChar[m])==0) 
          { Ignore=true;  break; }
      if (!Ignore) { j++;  k=0;  i++;  w.last++; }
      else { k=0;  i++; }
    }
  }
  w.item[j][k++]='\0';     // 把最后一个字符串收尾
  Ignore=false;
  for (int m=0; m<7; m++)
    if (strcmp(w.item[j],IgnoreChar[m])==0) 
      { Ignore=true;  break; }
  if (!Ignore) w.last++;   // 并把最大数加1
  return OK;
}

void GetLine(FILE *f) {
  fgets(buf, MaxLineLen, f);  // buf是全局数组变量
}

Status PutText(FILE *IdxFile, IdxListType MyIdx) { 
  int i,j,k;
  NLink p;
  for(i=0; i<MyIdx.last; i++) {
    for(j=0; j<MyIdx.item[i].key.length; j++)
      putc(*(MyIdx.item[i].key.ch+j ),IdxFile);
    putc('\t',IdxFile);
    if (MyIdx.item[i].key.length < 8) putc('\t',IdxFile);
    p = MyIdx.item[i].bnolist.head;
    for (k=0; k<MyIdx.item[i].bnolist.len; k++) {
      p = p->next;
      fprintf(IdxFile,"%03d",p->data);
      putc(' ', IdxFile);
    }
    putc('\n',IdxFile);
  }
  return OK;
}
	
void PrintFile(FILE *FileName) {  // 辅助函数
  char ch;
  rewind(FileName);
  ch=getc(FileName);
  while (ch!=EOF) {
    putchar(ch);
    ch=getc(FileName);
  }
  printf("\n");
  rewind(FileName);
}
