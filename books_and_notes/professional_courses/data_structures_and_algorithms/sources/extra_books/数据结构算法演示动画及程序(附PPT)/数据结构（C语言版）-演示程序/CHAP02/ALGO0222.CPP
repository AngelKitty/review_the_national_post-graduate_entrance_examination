Status cmp(PElemType a, PElemType b) {
  if (a.expn>=b.expn) return 1;
  else return 0;
}

void CreatPolyn(PLinkList &P, int m) {  // 算法2.22
  // 输入m项的系数和指数，建立表示一元多项式的有序链表P
  PLink h, q, s;
  PElemType e;
  int i;
  InitList(P);   h = GetHead(P);
  e.coef = 0.0;  e.expn = -1;
  SetCurElem(h, e);       // 设置头结点
  for (i=1; i<=m; ++i) {  // 依次输入m个非零项
    // scanf ("%f,%d\n",&e.coef, &e.expn);
    e.coef = (float)(random(1, 90) + random(10)/10.0);
    if (random(2)) e.coef = -e.coef;
    e.expn=e.expn+random(1,10); //产生随机的数据，但是expn值是递增的
    if (!LocateElem(P, e, q, cmp)) { // 当前链表中不存在该指数项
      if (MakeNode(s,e)) InsFirst(q, s);  // 生成结点并插入链表
      if(q==P.tail) P.tail=s;
    } else i--;  //  如果没有产生插入，则将i值减1
  }
} // CreatPolyn

Status PrintfPoly(PLinkList P) {
  int i=0;
  PLink q=P.head->next;
  while (q) {
    if (fabs(q->data.coef) > 0.005) {
      if (i>0) {
        if (q->data.coef>0.005) printf(" + ");
        else printf(" - ");
        printf("%.2f", fabs(q->data.coef));
      } else printf("%.2f", q->data.coef);
      if (q->data.expn>=1) printf("x");
      if (q->data.expn>1) printf("^%d", q->data.expn);
    }
    q=q->next;
    if (++i % 6 == 0) printf("\n     ");
  }
  printf("\n");
  return OK;
}