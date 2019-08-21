Status CreateGList(GList &L, SString S) {  // 算法5.7
  // 采用头尾链表存储结构，由广义表的书写形式串S创建广义表L。
  // 设emp="()"。
  char s[3]="()";
  SString emp;
  crt_SString(emp,s);
  SString sub,hsub;
  GList p,q;
  if (StrCompare(S, emp)) L = NULL;  // 创建空表
  else {
    if (!(L=(GList)malloc(sizeof(GLNode)))) return ERROR;  // 建表结点
    if (StrLength(S)==1) {  // 创建单原子广义表
       L->tag = ATOM;  L->atom =S[1]; }  
    } else { 
      L->tag = LIST;  p = L;  
      SubString(sub, S, 2, StrLength(S)-2);  // 脱外层括号 
      do {  // 重复建n个子表
        sever(sub, hsub);  // 从sub中分离出表头串 hsub
        CreateGList(p->ptr.hp, hsub);  q = p;
        if (!StrEmpty(sub)) {   // 表尾不空 
          if (!(p = (GLNode *) malloc (sizeof(GLNode)))) return ERROR;  
          p->tag = LIST;  q->ptr.tp = p;  
        }//if
      } while (!StrEmpty(sub));
      q->ptr.tp = NULL;
    } // else
  } // else
  return OK;
} // CreateGList
