Status CopyGList(GList &T, GList L) {  // 算法5.6
  // 采用头尾链表存储结构,由广义表L复制得到广义表T。
  if (!L) T = NULL;  // 复制空表
  else {
    if (!(T = (GList)malloc(sizeof(GLNode)))) // 建表结点
      return ERROR; 
    T->tag = L->tag;
    if (L->tag == ATOM) T->atom = L->atom;  // 复制单原子
    else { 
      CopyGList(T->ptr.hp, L->ptr.hp);
            // 复制广义表T->ptr.hp的副本L->ptr.hp
      CopyGList(T->ptr.tp, L->ptr.tp);
            // 复制广义表T->ptr.tp的副本L->ptr.tp
    } // else
  } // else
  return OK;
} // CopyGList
