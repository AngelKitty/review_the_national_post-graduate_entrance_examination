void CreateList_L(LinkList &L, int n) {  // 算法2.11
  // 逆位序输入（随机产生）n个元素的值，建立带表头结点的单链线性表L 
  LinkList p;
  int i;
  L = (LinkList)malloc(sizeof(LNode));
  L->next = NULL;              // 先建立一个带头结点的单链表
  for (i=n; i>0; --i) {
    p = (LinkList)malloc(sizeof(LNode));  // 生成新结点
    p->data = random(200);     // 改为一个随机生成的数字(200以内)
    p->next = L->next;    L->next = p;    // 插入到表头
  }
} // CreateList_L
