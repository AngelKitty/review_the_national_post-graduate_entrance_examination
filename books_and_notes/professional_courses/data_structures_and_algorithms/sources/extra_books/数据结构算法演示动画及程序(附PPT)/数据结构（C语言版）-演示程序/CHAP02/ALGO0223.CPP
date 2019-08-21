int Compare(PElemType a, PElemType b) {
  if (a.expn<b.expn) return -1;
  if (a.expn>b.expn) return 1;
  return 0;
}

void AddPolyn(PLinkList &Pa, PLinkList &Pb) {  // 算法2.23
  // 多项式加法：Pa = Pa＋Pb，利用两个多项式的结点构成"和多项式"。
  PLink ha,hb,qa,qb;
  PElemType a, b, temp;
  float sum;
  ha = GetHead(Pa);      // ha和hb分别指向Pa和Pb的头结点
  hb = GetHead(Pb);
  qa = NextPos(Pa,ha);   // qa和qb分别指向La和Lb中当前结点
  qb = NextPos(Pb,hb);
  while (qa && qb) {     // Pa和Pb均非空
    a = GetCurElem (qa); // a和b为两表中当前比较元素
    b = GetCurElem (qb);
    switch (Compare(a,b)) {
      case -1:  // 多项式PA中当前结点的指数值小
          ha = qa;
          qa = NextPos (Pa, qa);
          break;  
      case 0:   // 两者的指数值相等
          sum = a.coef + b.coef ;
          if (sum != 0.0) {  // 修改多项式PA中当前结点的系数值
            temp.coef=sum;
            temp.expn=a.expn;
            SetCurElem(qa, temp) ;
            ha = qa;
          } else {  // 删除多项式PA中当前结点
            DelFirst(ha, qa);
            FreeNode(qa);
          }
          DelFirst(hb, qb);
          FreeNode(qb);
          qb = NextPos(Pb, hb);
          qa = NextPos(Pa, ha);
          break;
      case 1:   // 多项式PB中当前结点的指数值小
          DelFirst(hb, qb);
          InsFirst(ha, qb); 
          qb = NextPos(Pb, hb);
          ha = NextPos(Pa, ha);
          break;
    } // switch
  } // while
  if (!Empty(Pb)) Append(Pa, qb);   // 链接Pb中剩余结点
  FreeNode(hb);  // 释放Pb的头结点
} // AddPolyn
